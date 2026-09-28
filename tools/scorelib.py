"""ตัวช่วยอ่านทะเบียน grader, ตารางคะแนน และหาโฟลเดอร์ของโจทย์

ใช้ร่วมกันโดย sync_done.py กับ find_missed.py

แหล่งความจริง 3 อย่าง
  1. tools/graders.txt       — มี contest อะไรบ้าง แต่ละอันโชว์ชื่อว่าอะไร
  2. tools/scores/<ชื่อ>.txt — ตารางคะแนนที่ก็อปมาจากหน้าเว็บของ contest นั้น
  3. problems/               — โฟลเดอร์โจทย์จริง (หารหัสโจทย์เจอเองโดยไม่ต้องรู้ว่าอยู่รอบไหน)

โจทย์โหลดมือจาก https://www.cs.science.cmu.ac.th/compo/ — ไม่มีสคริปต์ดึงอัตโนมัติ
"""

from __future__ import annotations

import os
import re
from dataclasses import dataclass

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOOLS_DIR = os.path.join(REPO_ROOT, "tools")
GRADERS_TXT = os.path.join(TOOLS_DIR, "graders.txt")
SCORES_DIR = os.path.join(TOOLS_DIR, "scores")
PROBLEMS_DIR = os.path.join(REPO_ROOT, "problems")
README_PATH = os.path.join(REPO_ROOT, "README.md")

DONE_DIRNAME = "_done"
SCORE_START = "<!-- SCORE:START -->"
SCORE_END = "<!-- SCORE:END -->"

# บรรทัดคะแนนจาก grader หน้าตาแบบนี้ (คั่นด้วย tab):
#   0 / 100	C2E17	Puyu Puyu (67-Exam1)	0.100 seconds	8.00 MiB	Batch	C2E17[.cpp|.c]
_LINE_RE = re.compile(r"^\s*(\d+)\s*/\s*(\d+)\s+([A-Za-z0-9_][A-Za-z0-9_.\-]*)\s*(.*)$")
_FIELD_SPLIT_RE = re.compile(r"\t+|\s{2,}")


# --------------------------------------------------------------------------
# contest / grader
# --------------------------------------------------------------------------
@dataclass
class Grader:
    name: str
    label: str = ""

    @property
    def title(self) -> str:
        return self.label or self.name

    @property
    def score_path(self) -> str:
        return os.path.join(SCORES_DIR, f"{self.name}.txt")

    @property
    def has_score(self) -> bool:
        return os.path.isfile(self.score_path)


def iter_graders() -> list[Grader]:
    """อ่าน tools/graders.txt → เรียงตามลำดับที่เขียนไว้ในไฟล์"""
    if not os.path.isfile(GRADERS_TXT):
        return []

    graders: list[Grader] = []
    with open(GRADERS_TXT, encoding="utf-8") as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            parts = _FIELD_SPLIT_RE.split(line, maxsplit=1)
            name = parts[0].strip()
            if not name:
                continue
            graders.append(Grader(name=name, label=parts[1].strip() if len(parts) > 1 else name))
    return graders


def get_grader(name: str) -> Grader | None:
    for g in iter_graders():
        if g.name == name:
            return g
    return None


# --------------------------------------------------------------------------
# คะแนน
# --------------------------------------------------------------------------
@dataclass
class Task:
    task_id: str
    score: int
    max_score: int
    name: str = ""
    time_limit: str = ""
    mem_limit: str = ""

    @property
    def is_full(self) -> bool:
        return self.max_score > 0 and self.score >= self.max_score

    @property
    def status(self) -> str:
        if self.max_score > 0 and self.score >= self.max_score:
            return "🟢"
        if self.score > 0:
            return "🟡"
        return "🔴"


def parse_score(path: str) -> list[Task]:
    """อ่านไฟล์ตารางคะแนน — บรรทัดที่จับรูปแบบไม่ได้ (หัวตาราง/คอมเมนต์) จะถูกข้าม"""
    if not os.path.isfile(path):
        raise FileNotFoundError(
            f"ไม่เจอ {os.path.relpath(path, REPO_ROOT)} — "
            "ก็อปตารางคะแนนจากหน้าเว็บมาวางก่อน"
        )

    tasks: list[Task] = []
    seen: set[str] = set()

    with open(path, encoding="utf-8") as f:
        for raw in f:
            m = _LINE_RE.match(raw.rstrip("\n"))
            if not m:
                continue

            score, max_score, task_id, rest = m.groups()
            if task_id in seen:
                continue
            seen.add(task_id)

            fields = [x.strip() for x in _FIELD_SPLIT_RE.split(rest.strip()) if x.strip()]
            tasks.append(
                Task(
                    task_id=task_id,
                    score=int(score),
                    max_score=int(max_score),
                    name=fields[0] if len(fields) > 0 else "",
                    time_limit=fields[1] if len(fields) > 1 else "",
                    mem_limit=fields[2] if len(fields) > 2 else "",
                )
            )

    return tasks


def parse_all_scores() -> list[tuple[Grader, list[Task]]]:
    """อ่านทุก contest ที่มีไฟล์คะแนนแล้ว → [(grader, tasks)] เฉพาะอันที่มีข้อมูลจริง"""
    out: list[tuple[Grader, list[Task]]] = []
    for g in iter_graders():
        if not g.has_score:
            continue
        tasks = parse_score(g.score_path)
        if tasks:
            out.append((g, tasks))
    return out


# --------------------------------------------------------------------------
# โฟลเดอร์โจทย์
# --------------------------------------------------------------------------
_index: dict[str, tuple[str, str, bool]] | None = None


def _task_index() -> dict[str, tuple[str, str, bool]]:
    """เดินทั่ว problems/ ครั้งเดียว → {รหัสโจทย์: (โฟลเดอร์แม่, path, อยู่ใน _done)}

    ไม่ต้องรู้ว่าข้อไหนอยู่รอบไหน — รหัสโจทย์ไม่ซ้ำกันอยู่แล้ว
    ถ้ารหัสเดียวกันโผล่ทั้งที่ยังไม่ย้ายและที่ _done/ แล้ว อันที่ยังไม่ย้ายชนะ
    """
    global _index
    if _index is not None:
        return _index

    index: dict[str, tuple[str, str, bool]] = {}
    if not os.path.isdir(PROBLEMS_DIR):
        _index = index
        return index

    # รอบแรก: โฟลเดอร์โจทย์ที่ยังไม่ถูกย้าย
    for root, dirs, _ in os.walk(PROBLEMS_DIR):
        dirs[:] = sorted(d for d in dirs if not d.startswith("."))
        if os.path.basename(root) == DONE_DIRNAME or root == PROBLEMS_DIR:
            continue
        for name in dirs:
            if name == DONE_DIRNAME:
                continue
            index.setdefault(name, (root, os.path.join(root, name), False))

    # รอบสอง: ของที่ย้ายเข้า _done/ แล้ว — ไม่ทับอันที่เพิ่งเจอ
    for root, dirs, _ in os.walk(PROBLEMS_DIR):
        if os.path.basename(root) != DONE_DIRNAME:
            continue
        parent = os.path.dirname(root)
        for name in sorted(dirs):
            if name.startswith("."):
                continue
            index.setdefault(name, (parent, os.path.join(root, name), True))

    _index = index
    return index


def reset_index() -> None:
    """ล้างแคช — ต้องเรียกหลังย้ายโฟลเดอร์ ไม่งั้น path ที่คืนมาจะเป็นของเก่า"""
    global _index
    _index = None


def find_task(task_id: str):
    """หาโฟลเดอร์โจทย์ → (โฟลเดอร์แม่, path เต็ม, อยู่ใน _done แล้วหรือยัง) หรือ None"""
    return _task_index().get(task_id)


def done_dir_for(grader_dir: str) -> str:
    done = os.path.join(grader_dir, DONE_DIRNAME)
    os.makedirs(done, exist_ok=True)
    return done
