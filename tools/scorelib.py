"""ตัวช่วยอ่าน tools/score.txt และหาโฟลเดอร์ของโจทย์

ใช้ร่วมกันโดย sync_done.py กับ find_missed.py
"""

from __future__ import annotations

import os
import re
from dataclasses import dataclass

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCORE_PATH = os.path.join(REPO_ROOT, "tools", "score.txt")
README_PATH = os.path.join(REPO_ROOT, "README.md")
INCAMP_DIR = os.path.join(REPO_ROOT, "problems", "2_incamp")

DONE_DIRNAME = "_done"
SCORE_START = "<!-- SCORE:START -->"
SCORE_END = "<!-- SCORE:END -->"

# บรรทัดคะแนนจาก grader หน้าตาแบบนี้ (คั่นด้วย tab):
#   0 / 100	C2E17	Puyu Puyu (67-Exam1)	0.100 seconds	8.00 MiB	Batch	C2E17[.cpp|.c]
_LINE_RE = re.compile(r"^\s*(\d+)\s*/\s*(\d+)\s+([A-Za-z0-9_][A-Za-z0-9_.\-]*)\s*(.*)$")
_FIELD_SPLIT_RE = re.compile(r"\t+|\s{2,}")


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


def parse_score(path: str = SCORE_PATH) -> list[Task]:
    """อ่าน score.txt — บรรทัดที่จับรูปแบบไม่ได้ (หัวตาราง/คอมเมนต์) จะถูกข้าม"""
    if not os.path.isfile(path):
        raise FileNotFoundError(
            f"ไม่เจอ {os.path.relpath(path, REPO_ROOT)} — "
            "ก็อปตารางคะแนนจากหน้า grader มาวางก่อน"
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


def iter_grader_dirs():
    """ไล่โฟลเดอร์ grader ทุกตัวใต้ problems/2_incamp/ → (ชื่อ, path)"""
    if not os.path.isdir(INCAMP_DIR):
        return
    for name in sorted(os.listdir(INCAMP_DIR)):
        path = os.path.join(INCAMP_DIR, name)
        if os.path.isdir(path) and not name.startswith("."):
            yield name, path


def find_task(task_id: str):
    """หาโฟลเดอร์โจทย์ → (ชื่อ grader, path เต็ม, อยู่ใน _done แล้วหรือยัง) หรือ None"""
    for grader, grader_dir in iter_grader_dirs():
        for in_done in (False, True):
            base = os.path.join(grader_dir, DONE_DIRNAME) if in_done else grader_dir
            path = os.path.join(base, task_id)
            if os.path.isdir(path):
                return grader, path, in_done
    return None


def done_dir_for(grader_dir: str) -> str:
    done = os.path.join(grader_dir, DONE_DIRNAME)
    os.makedirs(done, exist_ok=True)
    return done
