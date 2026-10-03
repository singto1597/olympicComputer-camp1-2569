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
# ชื่อโฟลเดอร์โจทย์
# --------------------------------------------------------------------------
# กติกา: โฟลเดอร์โจทย์ = <ลำดับ>_<รหัสโจทย์>_<ชื่อโจทย์>   เช่น 01_C1PC01_Alice_and_Bob
#        ลำดับมาจากลำดับบรรทัดใน tools/scores/<contest>.txt (ลำดับเดียวกับตารางใน README)
#        เพื่อให้เปิดโฟลเดอร์แล้วเรียงตามลำดับโจทย์ ไม่ใช่เรียงตามรหัส (C1PC → C1PE → C1PR)
#        ไฟล์ข้างในยังชื่อ <รหัสโจทย์>.cpp / <รหัสโจทย์>.pdf ตามที่ grader
#        บังคับชื่อไฟล์ตอน submit (คอลัมน์ Files) — ไม่ต่อชื่อโจทย์
_UNSAFE_PATH_RE = re.compile(r'[\\/:*?"<>|]')
_ORDER_PREFIX_RE = re.compile(r"^\d+_")  # เลขลำดับนำหน้าโฟลเดอร์: "01_C1PC01_x" → "C1PC01_x"
_SPACE_RUN_RE = re.compile(r"\s+")
_UNDERSCORE_RUN_RE = re.compile(r"_{2,}")


def folder_name_for(task_id: str, name: str = "", order: int | None = None) -> str:
    """ชื่อโฟลเดอร์ของโจทย์ 1 ข้อ → "<ลำดับ>_<รหัสโจทย์>_<ชื่อโจทย์>"

    order = ลำดับในไฟล์คะแนน (เริ่มที่ 1) ใส่เพื่อให้เรียงโฟลเดอร์ตามลำดับโจทย์
    ไม่ใส่ก็ได้ จะได้ชื่อแบบไม่มีเลขนำหน้า
    เลขรหัสที่ซ้ำอยู่ในชื่อจะถูกตัดทิ้ง ("C1PE01 - Constructor" → "Constructor")
    ถ้าแปลงแล้วไม่เหลือชื่อ เหลือแค่ <ลำดับ>_<รหัสโจทย์>
    """
    clean = (name or "").strip()
    if clean.upper().startswith(task_id.upper()):
        clean = clean[len(task_id):].lstrip(" -:–—")
    clean = _SPACE_RUN_RE.sub("_", clean)
    clean = _UNSAFE_PATH_RE.sub("", clean)
    clean = _UNDERSCORE_RUN_RE.sub("_", clean).strip("_")

    prefix = f"{order:02d}_" if order else ""
    return f"{prefix}{task_id}_{clean}" if clean else f"{prefix}{task_id}"


_known_ids_cache: set[str] | None = None


def known_task_ids() -> set[str]:
    """รหัสโจทย์ทั้งหมดที่อยู่ใน tools/scores/*.txt — ใช้เป็นพจนานุกรมตอนถอดรหัส"""
    global _known_ids_cache
    if _known_ids_cache is None:
        _known_ids_cache = {
            t.task_id for _grader, tasks in parse_all_scores() for t in tasks
        }
    return _known_ids_cache


def task_id_from_folder(folder_name: str, known_ids=None) -> str:
    """ถอดรหัสโจทย์จากชื่อโฟลเดอร์

    "01_C1PC01_Alice_and_Bob" → "C1PC01" · "C1PC01_Alice_and_Bob" → "C1PC01"
    · "C1PC01" → "C1PC01"

    รหัสโจทย์ส่วนใหญ่ไม่มี "_" จึงตัดที่ "_" ตัวแรกได้ แต่มีข้อยกเว้นจริงในค่าย
    (C1C0_ADD) ที่ตัดแบบนั้นแล้วเหลือ "C1C0" — จึงเทียบกับรหัสที่รู้จักก่อน
    แล้วเลือกรหัสที่ยาวที่สุดที่ชื่อโฟลเดอร์ขึ้นต้นด้วย (ไม่ส่ง known_ids มา
    ก็ดึงจากไฟล์คะแนนให้เอง) ไม่เข้าข่ายเลยจึงค่อยใช้วิธีตัดแบบเดิม
    """
    stripped = _ORDER_PREFIX_RE.sub("", folder_name)

    pool = known_task_ids() if known_ids is None else known_ids
    best = ""
    for task_id in pool:
        if stripped == task_id or stripped.startswith(task_id + "_"):
            if len(task_id) > len(best):
                best = task_id
    if best:
        return best

    # ไม่ตรงกับข้อไหนในไฟล์คะแนน (โฟลเดอร์แปลกปลอม) — ตัดแบบเดิม
    parts = folder_name.split("_")
    if len(parts) >= 2 and parts[0].isdigit():
        return parts[1]
    return parts[0]


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

    @property
    def old100_paths(self) -> list[str]:
        """ไฟล์รายการ 100 เก่าของ contest นี้ — <ชื่อ>.old100.txt หรือ <ชื่อ>.old100_68.txt

        ต่อท้ายด้วยรหัสปีที่โค้ดต้นทางมาจากได้ (68 = พ.ศ. 2568) ถ้ามีหลายปี
        ก็มีหลายไฟล์ได้ — อ่านรวมกันหมด
        """
        if not os.path.isdir(SCORES_DIR):
            return []
        prefix = f"{self.name}.old100"
        return [
            os.path.join(SCORES_DIR, n)
            for n in sorted(os.listdir(SCORES_DIR))
            if n.startswith(prefix) and n.endswith(".txt")
        ]

    @property
    def has_old100(self) -> bool:
        return bool(self.old100_paths)


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
# ป้าย "100 เก่า"
# --------------------------------------------------------------------------
def parse_old100(grader: Grader) -> set[str]:
    """รหัสโจทย์ที่ได้เต็มจากการส่งโค้ดปีก่อนหน้าซ้ำ (ไม่ได้เขียนใหม่ปีนี้)

    อ่านจาก tools/scores/<ชื่อ contest>.old100*.txt ข้าง ๆ ไฟล์คะแนน
    (ชื่อต่อท้ายด้วยปีที่โค้ดต้นทางมาจากได้ เช่น camp1_1.old100_68.txt)
    บรรทัดละ 1 รหัส · บรรทัดขึ้นต้นด้วย # คือคอมเมนต์ · ไม่มีไฟล์ = ไม่มีป้าย
    """
    ids: set[str] = set()
    for path in grader.old100_paths:
        with open(path, encoding="utf-8") as f:
            for raw in f:
                line = raw.strip()
                if not line or line.startswith("#"):
                    continue
                ids.add(line.split()[0])
    return ids


# ท้ายชื่อไฟล์บอกปีที่โค้ดต้นทางมาจาก: camp1_1.old100_68.txt → "68" (พ.ศ. 2568)
_OLD100_YEAR_RE = re.compile(r"\.old100_(\d+)\.txt$")


def old100_years(grader: Grader) -> list[str]:
    """ปี (พ.ศ. ย่อ) ของโค้ดต้นทางที่ติดป้าย 100 เก่า — เรียงจากน้อยไปมาก

    ไม่ได้ตั้งชื่อไฟล์ต่อท้ายปีไว้ก็ได้ → คืน [] แล้วผู้เรียกใช้คำกลาง ๆ แทน
    """
    years: list[str] = []
    for path in grader.old100_paths:
        m = _OLD100_YEAR_RE.search(os.path.basename(path))
        if m and m.group(1) not in years:
            years.append(m.group(1))
    return sorted(years, key=int)


# --------------------------------------------------------------------------
# โฟลเดอร์โจทย์
# --------------------------------------------------------------------------
_index: dict[str, tuple[str, str, bool]] | None = None


def _task_index() -> dict[str, tuple[str, str, bool]]:
    """เดินทั่ว problems/ ครั้งเดียว → {รหัสโจทย์: (โฟลเดอร์แม่, path, อยู่ใน _done)}

    ชื่อโฟลเดอร์มีชื่อโจทย์ต่อท้ายได้ (C1PC01_Alice_and_Bob) จึงถอดรหัสด้วย
    task_id_from_folder() ไม่ใช่ใช้ชื่อโฟลเดอร์ตรง ๆ เป็นคีย์
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
            index.setdefault(
                task_id_from_folder(name), (root, os.path.join(root, name), False)
            )

    # รอบสอง: ของที่ย้ายเข้า _done/ แล้ว — ไม่ทับอันที่เพิ่งเจอ
    for root, dirs, _ in os.walk(PROBLEMS_DIR):
        if os.path.basename(root) != DONE_DIRNAME:
            continue
        parent = os.path.dirname(root)
        for name in sorted(dirs):
            if name.startswith("."):
                continue
            index.setdefault(
                task_id_from_folder(name), (parent, os.path.join(root, name), True)
            )

    _index = index
    return index


def reset_index() -> None:
    """ล้างแคช — ต้องเรียกหลังย้ายโฟลเดอร์/เพิ่มไฟล์คะแนน ไม่งั้นของเก่าจะค้าง"""
    global _index, _known_ids_cache
    _index = None
    _known_ids_cache = None


def find_task(task_id: str):
    """หาโฟลเดอร์โจทย์ → (โฟลเดอร์แม่, path เต็ม, อยู่ใน _done แล้วหรือยัง) หรือ None"""
    return _task_index().get(task_id)


def done_dir_for(grader_dir: str) -> str:
    done = os.path.join(grader_dir, DONE_DIRNAME)
    os.makedirs(done, exist_ok=True)
    return done
