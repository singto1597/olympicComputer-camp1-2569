#!/usr/bin/env python3
"""คอมไพล์ + รันโจทย์ C++ ในเครื่อง — สคริปต์กลางตัวเดียว ทั้ง terminal และ VS Code task

สคริปต์นี้ **ดูจากเนื้อในโฟลเดอร์** ว่าโจทย์ข้อนี้มีอะไร แล้วค่อยตัดสินใจ — ไม่ได้เดาจาก
ชื่อไฟล์/ชื่อโฟลเดอร์อย่างเดียว จึงใช้ได้กับทุกทรงที่เจอจริงในเรพ

  1. โจทย์แบบฟังก์ชัน — มีโฟลเดอร์ของ grader (อะไรก็ได้ที่ข้างในมี .h หรือ .cpp ที่มี main)
     เราเขียนแค่ตัวฟังก์ชันในไฟล์ .cpp ชั้นบนสุดของโฟลเดอร์โจทย์ ชื่อตามคอลัมน์ Files
     (บางข้อไม่ตรงกับรหัส Task เช่น C1C02 → exchange.cpp · F01 → cube_abs.cpp)
     **จับคู่ "ไฟล์เรา ↔ โฟลเดอร์ grader" จาก `#include "…"` ในไฟล์เราเป็นหลัก** จึงแยก
     `provided/` กับ `provided_yrc/` ที่อยู่ในโฟลเดอร์เดียวกันออกจากกันได้ (ดู C1T49)
  2. โจทย์ไฟล์เดียว — <ID>.cpp มี main ในตัว ไม่ต้องมี grader
  3. โจทย์ที่ไฟล์ชั้นบนสุดไม่มี main แต่มีตัวรันอยู่ใน local/ — สคริปต์จะบอกให้ใช้ --file

ตัวอย่าง
  python3 tools/run.py C1T43                  # หาโฟลเดอร์จากรหัส แล้วคอมไพล์ + รัน
  python3 tools/run.py C1T43 -b               # คอมไพล์อย่างเดียว
  python3 tools/run.py C1T43 --sample 1       # รันด้วย sample ที่ 1 (+เทียบ .out ถ้ามี)
  python3 tools/run.py C1T43 --all-samples    # วนทุก sample แล้วสรุปผ่านกี่อัน
  python3 tools/run.py C1T43 --stdin in.txt --out res.txt
  python3 tools/run.py --file "<path>/C1T43.cpp"   # วิธีที่ tasks.json ใช้ (${file})
  python3 tools/run.py --here                 # ใช้โฟลเดอร์โจทย์ที่ครอบ cwd อยู่
  python3 tools/run.py C1T38 --with local/C1T38_main.cpp
  python3 tools/run.py C1T43 --asan           # AddressSanitizer + UBSan (หาล้น/oob)
  python3 tools/run.py C1T43 -- 2 3           # ส่ง argument ที่เหลือให้โปรแกรม
  python3 tools/run.py C1T43 --list           # โชว์ layout/ไฟล์/sample แล้วจบ
  python3 tools/run.py C1T43 --clean          # ลบ .run/ ของข้อนี้

ของที่ออกมา
  <repo>/input.in        input ที่เราเองก็อปมาใส่ (ราก repo — ไฟล์เดียวใช้ทุกข้อ)
  <repo>/output.txt      stdout ของครั้งล่าสุด
  <โฟลเดอร์โจทย์>/.run/  bin (โปรแกรมที่คอมไพล์แล้ว) · <sample>.got (sample ที่เทียบไม่ผ่าน)

⚠️ input.in ใช้ร่วมทุกข้อเพราะทำทีละข้ออยู่แล้ว — สคริปต์จะพิมพ์ทุกครั้งว่าใช้ input ไฟล์ไหน
   ถ้าเปลี่ยนข้อแล้วลืมเปลี่ยน input มันจะเงียบ ๆ ใช้ของเก่า
"""

from __future__ import annotations

import argparse
import glob
import os
import re
import shlex
import shutil
import subprocess
import sys
import time
from dataclasses import dataclass, field

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import PROBLEMS_DIR, REPO_ROOT, find_task, task_id_from_folder  # noqa: E402

IS_WINDOWS = sys.platform.startswith("win")
EXE_SUFFIX = ".exe" if IS_WINDOWS else ""
RUN_DIRNAME = ".run"
BIN_NAME = "bin"
TEMPLATE_PATH = os.path.join(REPO_ROOT, "template.cpp")

# ไฟล์ input/output ที่ใช้ซ้อม — อยู่ราก repo ที่เดียว (ผู้ใช้ขอ: แท็บเดียวอยู่ตลอด ก็อป
# input ทับได้เลย ไม่ต้องเข้าไปหาใน .run/ และไม่หายตอน --clean)
INPUT_NAME = "input.in"
OUTPUT_NAME = "output.txt"
DEFAULT_STDIN = os.path.join(REPO_ROOT, INPUT_NAME)
DEFAULT_STDOUT = os.path.join(REPO_ROOT, OUTPUT_NAME)

# โฟลเดอร์ของ grader — ตัวจริงตัดสินจาก "เนื้อใน" (มี .h หรือ .cpp ที่มี main)
# ชื่อพวกนี้ใช้แค่จัดลำดับความสำคัญ + กันไม่ให้ถูกนับเป็นโฟลเดอร์โจทย์
# ⚠️ ต้องเทียบแบบ prefix: ของจริงมี `provided_yrc` ซึ่งไม่ใช่ทั้ง `provided` และไม่ใช่
#    โฟลเดอร์โจทย์ — เทียบแบบเท่ากันเป๊ะเมื่อไหร่ มันจะกลายเป็น "โฟลเดอร์โจทย์" ทันที
GRADER_EXACT = ("provided", "attachment")
GRADER_PREFIX = ("provided_", "attachment_")
# โฟลเดอร์เสริมที่ไม่ใช่ทั้งโฟลเดอร์โจทย์และโฟลเดอร์ grader
# (local/ = ไฟล์ซ้อมของเรา — มี main ได้ แต่เป็นไฟล์ครบในตัว ห้ามเอาไปเชื่อมกับใคร)
AUX_DIRNAMES = ("local", "sample", "testcases")
SAMPLE_DIRNAMES = ("sample", "testcases")

EXPECTED_EXTS = (".out", ".ans")
DEFAULT_STD = "c++17"
DEFAULT_TIMEOUT = 10.0

# ไฟล์ซ้อม/เวอร์ชันลอง — ไม่ใช่ไฟล์ที่ส่ง
# _v?\d+ ไว้กับ C1T20_2.cpp · ที่เหลือคือของใน local/
VARIANT_RE = re.compile(r"_(?:singlefile|localtest\w*|af|TA-Teaching|optimize|v?\d+)$", re.I)
MAIN_RE = re.compile(r"^[^\S\n]*(?:int|void|signed)\s+main\s*\(", re.M)
INCLUDE_RE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"', re.M)
STD_RE = re.compile(r"-std=(gnu\+\+|c\+\+)(\d+)")
NUM_RE = re.compile(r"(\d+)")

# แคชต่อการรันหนึ่งครั้ง — โฟลเดอร์โจทย์เดียวกันถูกอ่านซ้ำหลายรอบ (หา main ทีละไฟล์)
_TEXT_CACHE: dict[str, str] = {}
_MAIN_CACHE: dict[str, bool] = {}


class Fail(Exception):
    """ปัญหาที่อธิบายให้ผู้ใช้เข้าใจได้ — พิมพ์ข้อความแล้วจบ ไม่ต้อง traceback"""


# ---------------------------------------------------------------- ยูทิลิตี้
def rel(path: str) -> str:
    try:
        return os.path.relpath(path, REPO_ROOT)
    except ValueError:  # คนละ drive บน Windows
        return path


def read_text(path: str) -> str:
    if path not in _TEXT_CACHE:
        try:
            with open(path, encoding="utf-8", errors="replace") as f:
                _TEXT_CACHE[path] = f.read()
        except OSError:
            _TEXT_CACHE[path] = ""
    return _TEXT_CACHE[path]


def has_main(path: str) -> bool:
    if path not in _MAIN_CACHE:
        _MAIN_CACHE[path] = bool(MAIN_RE.search(read_text(path)))
    return _MAIN_CACHE[path]


def quoted_includes(path: str) -> list[str]:
    """ชื่อ header ที่ไฟล์นี้ #include แบบ quote — สัญญาณหลักที่ใช้จับคู่กับ grader"""
    return INCLUDE_RE.findall(read_text(path))


def same_bytes(a: str, b: str) -> bool:
    try:
        with open(a, "rb") as fa, open(b, "rb") as fb:
            return fa.read() == fb.read()
    except OSError:
        return False


def stem(path: str) -> str:
    return os.path.splitext(os.path.basename(path))[0]


# ---- ชื่อโฟลเดอร์: grader / โฟลเดอร์เสริม ----
def is_grader_dirname(name: str) -> bool:
    return name in GRADER_EXACT or name.startswith(GRADER_PREFIX)


def is_aux_dirname(name: str) -> bool:
    return name in AUX_DIRNAMES


def is_support_dirname(name: str) -> bool:
    """โฟลเดอร์เสริม = ไม่ใช่โฟลเดอร์โจทย์ (ของ grader · ของซ้อม · sample · .run)"""
    return is_grader_dirname(name) or is_aux_dirname(name) or name == RUN_DIRNAME


def _parts(path: str) -> list[str]:
    return os.path.abspath(path).replace("\\", "/").split("/")


def in_grader_dir(path: str) -> bool:
    return any(is_grader_dirname(p) for p in _parts(path))


def in_aux_dir(path: str) -> bool:
    return any(is_aux_dirname(p) for p in _parts(path))


# ---- อ่านโฟลเดอร์ ----
def listdir_files(d: str) -> list[str]:
    try:
        return sorted(n for n in os.listdir(d) if os.path.isfile(os.path.join(d, n)))
    except OSError:
        return []


def listdir_dirs(d: str) -> list[str]:
    """โฟลเดอร์ลูก — ข้ามของที่ขึ้นต้นด้วย "." (รวม .run) และไฟล์"""
    try:
        return sorted(n for n in os.listdir(d)
                      if not n.startswith(".") and os.path.isdir(os.path.join(d, n)))
    except OSError:
        return []


def search_dirs(pdir: str, depth: int = 2) -> list[str]:
    """โฟลเดอร์ใต้ pdir ที่จะค้นหา header — pdir เอง แล้วไล่ลงไปไม่เกิน depth ชั้น"""
    out = [pdir]
    for _ in range(depth):
        for d in list(out):
            for n in listdir_dirs(d):
                nd = os.path.join(d, n)
                if nd not in out:
                    out.append(nd)
    return out


def find_header(name: str, pdir: str) -> list[str]:
    """ทุกโฟลเดอร์ใต้ pdir ที่มีไฟล์ชื่อ name (ตื้น → ลึก)"""
    return [d for d in search_dirs(pdir) if os.path.isfile(os.path.join(d, name))]


def dir_contains_file(d: str, name: str, depth: int = 1) -> bool:
    """d หรือโฟลเดอร์ย่อยของ d (ไม่เกิน depth ชั้น) มีไฟล์ชื่อ name ไหม"""
    if os.path.isfile(os.path.join(d, name)):
        return True
    if depth <= 0:
        return False
    return any(dir_contains_file(s, name, depth - 1) for s in listdir_dirs(d))


def header_stems(d: str) -> list[str]:
    return [os.path.splitext(n)[0] for n in listdir_files(d)
            if n.lower().endswith((".h", ".hpp"))]


# ---------------------------------------------------------------- หาโฟลเดอร์โจทย์
def looks_like_problem_dir(d: str) -> bool:
    if not os.path.isdir(d):
        return False
    name = os.path.basename(d)
    if name.startswith(".") or is_support_dirname(name):
        return False
    names = listdir_files(d)
    return any(n.lower().endswith(".pdf") for n in names) or any(
        n.lower().endswith(".cpp") for n in names
    )


def under_problems(path: str) -> bool:
    """อยู่ใน problems/ จริง ๆ ไหม — เทียบขอบ "/" กัน problems_foo หลอก"""
    p = os.path.abspath(path)
    return p == PROBLEMS_DIR or p.startswith(PROBLEMS_DIR + os.sep)


def problem_dir_from_path(path: str) -> str | None:
    """ไล่จากไฟล์/โฟลเดอร์ขึ้นไปจนเจอโฟลเดอร์โจทย์ (ข้าม provided*/ local/ sample/ ...)"""
    cur = os.path.abspath(path)
    if not os.path.isdir(cur):
        cur = os.path.dirname(cur)
    while under_problems(cur):
        if looks_like_problem_dir(cur):
            return cur
        parent = os.path.dirname(cur)
        if parent == cur:
            break
        cur = parent
    return None


def find_problem_by_id(task_id: str) -> str:
    hit = find_task(task_id)
    if hit:
        return hit[1]
    hits = []
    for root, dirs, _ in os.walk(PROBLEMS_DIR):
        dirs[:] = sorted(d for d in dirs
                         if not d.startswith(".") and not is_support_dirname(d))
        for name in dirs:
            if task_id.lower() in name.lower():
                hits.append(os.path.join(root, name))
    if len(hits) == 1:
        return hits[0]
    if hits:
        raise Fail(
            f"เจอหลายโฟลเดอร์ที่ตรงกับ {task_id} — ระบุให้ชัด:\n"
            + "\n".join("   " + rel(h) for h in hits)
        )
    raise Fail(
        f"ไม่เจอโจทย์รหัส {task_id}\n"
        "   · รหัสต้องตรงกับคอลัมน์ Task ใน tools/scores/*.txt\n"
        '   · หรือส่ง path ไฟล์ก็ได้: python3 tools/run.py --file "<path>.cpp>"'
    )


# ---------------------------------------------------------------- หาโฟลเดอร์ grader
def grader_rank(name: str) -> tuple[int, str]:
    """จัดลำดับเพื่อโชว์/ใช้ตอนกำกวมเท่านั้น — **ไม่ใช้เดาการจับคู่**"""
    if name == "provided":
        return (0, name)
    if name.startswith("provided_"):
        return (1, name)
    if name == "attachment":
        return (2, name)
    if name.startswith("attachment_"):
        return (3, name)
    return (4, name)


def grader_dirs(pdir: str) -> list[str]:
    """โฟลเดอร์ของ grader = โฟลเดอร์ลูกที่มี .h **หรือ** .cpp ที่มี main

    ตัดสินจากเนื้อในไม่ใช่ชื่อ → ใช้ได้กับ `provided`, `provided_yrc`, `attachment`
    และชื่ออะไรก็ได้ที่ผู้ใช้ตั้งมา (ผู้ใช้ขี้เกียจ rename)
    `local/` ถูกตัดออกโดยชื่อ เพราะข้างในมี .cpp ที่มี main เหมือนกัน แต่เป็นไฟล์ครบในตัว
    (เอาไปเชื่อมกับไฟล์เราเมื่อไหร่ = สัญลักษณ์ซ้ำ)
    """
    out = []
    for n in listdir_dirs(pdir):
        if is_aux_dirname(n):
            continue
        d = os.path.join(pdir, n)
        files = listdir_files(d)
        if any(f.lower().endswith((".h", ".hpp")) for f in files) or any(
            f.lower().endswith(".cpp") and has_main(os.path.join(d, f)) for f in files
        ):
            out.append(d)
    return sorted(out, key=lambda d: grader_rank(os.path.basename(d)))


def grader_dir_containing(path: str, gdirs: list[str]) -> str | None:
    """โฟลเดอร์ grader ที่ไฟล์นี้อยู่ข้างใน (ใช้ตอนเปิดไฟล์ของ grader อยู่)"""
    p = os.path.abspath(path)
    for g in gdirs:
        if p == g or p.startswith(g + os.sep):
            return g
    return None


# ---------------------------------------------------------------- หาไฟล์ที่เราแก้
def user_sources(pdir: str) -> list[str]:
    """ไฟล์ .cpp ที่เป็นของเรา = ชั้นบนสุดของโฟลเดอร์โจทย์ (ของ grader อยู่ในโฟลเดอร์แยก)"""
    return [os.path.join(pdir, n) for n in listdir_files(pdir) if n.lower().endswith(".cpp")]


def sources_with_main(pdir: str, skip: list[str] | None = None) -> list[str]:
    """ทุก .cpp ใต้ pdir (ลึกไม่เกิน 1 ชั้น) ที่มี main — ใช้เสนอทางออกตอน error"""
    skip = skip or []
    out = []
    for d in [pdir] + [os.path.join(pdir, n) for n in listdir_dirs(pdir)]:
        if os.path.abspath(d) in skip:
            continue
        for n in listdir_files(d):
            if n.lower().endswith(".cpp") and has_main(os.path.join(d, n)):
                out.append(os.path.join(d, n))
    return out


def pair_score(cand: str, gdirs: list[str], tid: str) -> int:
    """คะแนนการจับคู่ของไฟล์ candidate กับโฟลเดอร์ grader (ยิ่งมากยิ่งดี)

    อันดับ 1: #include "…" ของไฟล์นี้ไปเจอในโฟลเดอร์ grader
    อันดับ 2: ชื่อไฟล์ตรงกับชื่อ .h ของ grader
    อันดับ 3: ชื่อไฟล์ตรงกับรหัสโจทย์
    """
    incs = quoted_includes(cand)
    if any(dir_contains_file(g, h) for g in gdirs for h in incs):
        return 3
    s = stem(cand)
    if tid and s == tid:
        return 2
    if any(s == hs for g in gdirs for hs in header_stems(g)):
        return 2
    return 0


def pick_user_source(pdir: str, gdirs: list[str], explicit: str, opened: str | None) -> tuple[str, str]:
    """เลือกไฟล์ที่เราแก้ + บอกเหตุผลที่เลือก (คืน (path, เหตุผล))"""
    if explicit:
        p = os.path.abspath(explicit)
        if not os.path.isfile(p):
            raise Fail(f"ไม่เจอไฟล์ {explicit}")
        return p, "ระบุมาด้วย --file"

    cands = user_sources(pdir)
    if not cands:
        raise Fail(
            f"ไม่เจอไฟล์ .cpp ใน {rel(pdir)}\n"
            "   ไฟล์ที่เราแก้ต้องอยู่ชั้นบนสุดของโฟลเดอร์โจทย์ (ของ grader อยู่ในโฟลเดอร์แยก)"
        )
    if len(cands) == 1:
        return cands[0], "มีไฟล์ .cpp เดียวในโฟลเดอร์"

    tid = task_id_from_folder(os.path.basename(pdir))

    # เปิดไฟล์ของ grader อยู่ → เลือกไฟล์ที่จับคู่กับโฟลเดอร์นั้น
    if opened:
        hits = [c for c in cands
                if any(dir_contains_file(opened, h) for h in quoted_includes(c))]
        if len(hits) == 1:
            return hits[0], f"จับคู่กับ {rel(opened)} ที่เปิดอยู่"

    for label, key in (
        (f"ชื่อไฟล์ตรงกับรหัสโจทย์ {tid}", lambda c: bool(tid) and stem(c) == tid),
        ("#include ไปเจอในโฟลเดอร์ grader", lambda c: pair_score(c, gdirs, tid) == 3),
        ("ไม่ใช่ไฟล์เวอร์ชันลอง (_optimize / _2 / …)", lambda c: not VARIANT_RE.search(stem(c))),
    ):
        hits = [c for c in cands if key(c)]
        if len(hits) == 1:
            return hits[0], label
        if hits:
            cands = hits

    names = " · ".join(os.path.basename(c) for c in cands)
    raise Fail(
        f"เลือกไฟล์ที่เราแก้ไม่ได้ — มีหลายไฟล์ที่เข้าเกณฑ์พอกัน: {names}\n"
        f"   ใน {rel(pdir)} ไม่มีสัญญาณพอจะบอกว่าไฟล์ไหนคือคำตอบ\n"
        '     • ระบุให้ชัด: python3 tools/run.py --file "<ชื่อไฟล์>"'
    )


# ---------------------------------------------------------------- จับคู่กับ grader
def pick_grader_dir(user_src: str, gdirs: list[str], opened: str | None,
                    pdir: str, need: bool) -> tuple[str | None, str, list[str]]:
    """เลือกโฟลเดอร์ grader ที่เข้าคู่กับไฟล์เรา — คืน (โฟลเดอร์, เหตุผล, คำเตือน)

    ลำดับ: ไฟล์ที่เปิดอยู่ → #include ของเรา → ชื่อ .h ตรงกับชื่อไฟล์ → รหัสโจทย์
           → เหลือโฟลเดอร์เดียวก็ใช้เลย
    **กำกวม = พังทันที** ไม่เดา — เดาผิดแล้วคอมไพล์ผ่านคือกรณีที่แย่ที่สุด (ได้คำตอบผิดเงียบ ๆ)
    """
    warns: list[str] = []
    if not gdirs:
        if need:
            raise _no_grader_error(user_src, pdir)
        return None, "", warns

    if opened:
        return opened, f"ไฟล์ที่เปิดอยู่ ({rel(opened)})", warns

    incs = quoted_includes(user_src)
    hits = [g for g in gdirs if any(dir_contains_file(g, h) for h in incs)]

    if len(hits) > 1:
        shown = "\n".join("     " + rel(g) for g in hits)
        raise Fail(
            f"จับคู่ไฟล์เรากับ grader ไม่ได้ — {os.path.basename(user_src)} #include "
            f'"{incs[0]}" แล้วเจอมากกว่าหนึ่งที่\n'
            f"   ที่เจอ:\n{shown}\n"
            "   ปกติแต่ละโจทย์มี .h ของตัวเองชื่อเดียว ถ้าเจอซ้ำแปลว่าตั้งชื่อไฟล์ซ้ำกัน\n"
            "     • แก้ชื่อ .h ให้ต่างกัน (ตามที่โจทย์ให้มา)\n"
            "     • ถ้ารู้อยู่แล้วว่าต้องใช้ตัวไหน เปิดไฟล์ของเราชั้นบนสุดแล้วกด task อีกครั้ง"
        )
    if len(hits) == 1:
        return hits[0], f'#include "{incs[0]}" ไปเจอที่นี่', warns

    s = stem(user_src)
    stem_hits = [g for g in gdirs if s in header_stems(g)]
    if len(stem_hits) == 1:
        return stem_hits[0], f"ชื่อไฟล์ตรงกับ .h ของ grader ({s}.h)", warns

    tid = task_id_from_folder(os.path.basename(pdir))
    tid_hits = [g for g in gdirs if tid and tid in header_stems(g)]
    if len(tid_hits) == 1:
        return tid_hits[0], f"รหัสโจทย์ {tid} ตรงกับ .h ของ grader", warns

    if len(gdirs) == 1:
        if incs or need:
            warns.append(
                f"จับคู่จากชื่อโฟลเดอร์ (ไม่ได้ดู #include) — เจอโฟลเดอร์ grader แค่ที่เดียว: "
                f"{rel(gdirs[0])}"
            )
        return gdirs[0], "เหลือโฟลเดอร์ grader เดียว", warns

    if not need:
        # ไฟล์เรามี main อยู่แล้ว → ไม่ต้องเชื่อมใคร -I ไม่จำเป็น
        return None, "", warns

    found = "\n".join("     " + rel(g) + f" (มี {', '.join(header_stems(g)) or '—'})"
                      for g in gdirs)
    raise Fail(
        f"หา grader ที่เข้าคู่กับ {os.path.basename(user_src)} ไม่เจอ\n"
        f"   ไฟล์เราสั่ง #include {incs or '(ไม่มี)'} แต่ไม่มีโฟลเดอร์ไหนมี header พวกนั้น\n"
        f"   โฟลเดอร์ grader ที่เจอ:\n{found}\n"
        "     • ตรวจว่าลอก .h ของ grader มาครบหรือยัง\n"
        "     • ถ้าโจทย์ข้อนี้ให้เขียน main เอง ให้เขียน main ในไฟล์เราแล้วรันใหม่"
    )


def _no_grader_error(user_src: str, pdir: str) -> Fail:
    """ไฟล์เราไม่มี main และไม่มีโจ folder ของ grader — เสนอทางออกที่จับต้องได้"""
    lines = [f"{os.path.basename(user_src)} ไม่มี main และโฟลเดอร์นี้ไม่มี grader ให้"]
    lines.append("   โจทย์ข้อนี้ต้องมี main เอง — เลือกทางใดทางหนึ่ง")
    lines.append(f"     • เขียน main ใน {os.path.basename(user_src)} แล้วรันใหม่")
    others = [p for p in sources_with_main(pdir) if os.path.abspath(p) != os.path.abspath(user_src)]
    if others:
        # quote ไว้เสมอ — ชื่อโฟลเดอร์จริงมี `!` (fish ตีเป็น history) และ `,`
        lines.append('     • ใช้ไฟล์ที่รันได้อยู่แล้ว: '
                     f'python3 tools/run.py --file "{rel(others[0])}"')
    lines.append('     • คอมไพล์ไฟล์อื่นเพิ่มด้วย: --with "<ไฟล์ที่มี main>"')
    return Fail("\n".join(lines))


def resolve_includes(sources: list[str], pdir: str, paired: str | None) -> tuple[list[str], list[str]]:
    """หา -I ที่จำเป็นจริง จาก #include ของทุกไฟล์ที่คอมไพล์ — คืน (โฟลเดอร์, คำเตือน)"""
    dirs: list[str] = []
    if paired:
        dirs.append(paired)
    warns: list[str] = []
    for src in sources:
        for h in quoted_includes(src):
            hits = find_header(h, pdir)
            if not hits:
                continue
            first = hits[0]
            if first not in dirs:
                dirs.append(first)
            shadow = [d for d in hits[1:] if d == pdir]
            if shadow and paired:
                warns.append(
                    f"{h} มีทั้งที่ชั้นบนสุดของโฟลเดอร์โจทย์และใน {rel(first)} — "
                    "g++ จะเลือกตัวที่อยู่ใกล้ไฟล์ที่ #include ถ้าไม่ตั้งใจให้ลบตัวที่ซ้ำ"
                )
    return dirs, warns


# ---------------------------------------------------------------- sample
def sample_dirs(pdir: str, paired: str | None, gdirs: list[str]) -> list[str]:
    """โฟลเดอร์ที่อาจมี sample เรียงตามความสำคัญ (เจอที่แรกชนะ)"""
    out: list[str] = []

    def add(d: str | None) -> None:
        if d and os.path.isdir(d) and d not in out:
            out.append(d)

    for n in SAMPLE_DIRNAMES:                       # 1) ชั้นบนสุดของโฟลเดอร์โจทย์
        add(os.path.join(pdir, n))
    for g in ([paired] if paired else []) + [d for d in gdirs if d != paired]:
        for n in SAMPLE_DIRNAMES:                   # 2) ข้างในโฟลเดอร์ grader
            add(os.path.join(g, n))
    for d in search_dirs(pdir):                     # 3) ที่อื่นใต้ pdir (ชื่อ grader อะไรก็ได้)
        if os.path.basename(d) in SAMPLE_DIRNAMES:
            add(d)
    add(pdir)                                       # 4) ไฟล์ .in หลวม ๆ ในโฟลเดอร์โจทย์
    return out


def find_sample_dir(pdir: str, paired: str | None, gdirs: list[str]) -> str | None:
    for d in sample_dirs(pdir, paired, gdirs):
        if glob.glob(os.path.join(d, "*.in")):
            return d
    return None


def collect_samples(pdir: str, paired: str | None, gdirs: list[str]) -> list[dict]:
    d = find_sample_dir(pdir, paired, gdirs)
    if not d:
        return []
    out = []
    for n in sorted(glob.glob(os.path.join(d, "*.in"))):
        name = os.path.basename(n)[:-3]
        exp = None
        for ext in EXPECTED_EXTS:
            cand = os.path.join(d, name + ext)
            if os.path.isfile(cand):
                exp = cand
                break
        m = NUM_RE.search(name)
        out.append({"name": name, "index": int(m.group(1)) if m else None, "in": n, "out": exp})
    return out


def detect_std(pdir: str, paired: str | None, gdirs: list[str]) -> str:
    """อ่าน -std= จากสคริปต์คอมไพล์ที่ grader ให้มา (AplusB/Georgette = gnu++20)

    อ่านจากโฟลเดอร์ grader ที่จับคู่ได้ก่อน — สำคัญกับ C1T49 ที่ provided/ ใช้ gnu++17
    แต่ provided_yrc/ ใช้ gnu++20
    """
    order = ([paired] if paired else []) + [d for d in gdirs if d != paired] + [pdir]
    for d in order:
        for n in listdir_files(d):
            if n.startswith(("compile", "build")) and n.endswith((".sh", ".bat")):
                m = STD_RE.search(read_text(os.path.join(d, n)))
                if m:
                    return m.group(1) + m.group(2)
    return DEFAULT_STD


# ---------------------------------------------------------------- วางแผน
@dataclass
class Plan:
    pdir: str
    user_src: str
    kind: str                 # function | single
    sources: list[str]
    includes: list[str]
    std: str
    samples: list[dict]
    run_dir: str
    binary: str
    grader_dir: str | None = None
    paired_by: str = ""
    picked_by: str = ""
    notes: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)


def make_plan(args: argparse.Namespace) -> Plan:
    hint = (args.file or "").strip()
    target = (args.target or "").strip()

    # 1) หาโฟลเดอร์โจทย์
    pdir = problem_dir_from_path(hint) if hint else None
    if pdir is None and target:
        pdir = problem_dir_from_path(target) if target.lower().endswith(".cpp") else find_problem_by_id(target)
    if pdir is None and (args.here or (not hint and not target)):
        pdir = problem_dir_from_path(os.getcwd())
    if pdir is None:
        # ไม่อยู่ใน problems/ เลย → คอมไพล์ไฟล์เดียวแบบหลวม ๆ (เช่น free.cpp ที่ราก repo)
        if hint and hint.lower().endswith(".cpp"):
            pdir = os.path.dirname(os.path.abspath(hint))
        else:
            raise Fail(
                "หาโฟลเดอร์โจทย์ไม่เจอ\n"
                "   · เปิดไฟล์ .cpp ของโจทย์แล้วกด task / หรือรัน `python3 tools/run.py C1T43`\n"
                "   · หรือ cd เข้าโฟลเดอร์โจทย์แล้วใช้ `--here`"
            )

    notes: list[str] = []
    warnings: list[str] = []
    gdirs = grader_dirs(pdir)

    # 2) ไฟล์ที่เราแก้ — ของในโฟลเดอร์ grader ไม่นับ (เป็นเบาะแสว่ากำลังเปิดไฟล์ใครอยู่)
    #    ส่วนไฟล์ใน local/ ใช้ได้เลย — เป็นไฟล์ซ้อมของเราเอง
    explicit = hint if (hint.lower().endswith(".cpp") and not in_grader_dir(hint)) else ""
    opened = grader_dir_containing(hint, gdirs) if hint else None
    user_src, picked_by = pick_user_source(pdir, gdirs, explicit, opened)

    user_has_main = has_main(user_src)

    # --with ต้องถูกเก็บ "ก่อน" เช็คว่ามี main ไหม — มันคือทางออกที่ข้อความ error ของเรา
    # แนะนำเอง (`--with "<ไฟล์ที่มี main>"`) ถ้าไปเก็บทีหลัง ทางออกนั้นจะใช้ไม่ได้จริง
    sources = [user_src]
    for w in args.with_:
        wp = os.path.abspath(w)
        if not os.path.isfile(wp):
            raise Fail(f"ไม่เจอไฟล์ --with {w}")
        if wp not in sources:
            sources.append(wp)
    have_main = any(has_main(p) for p in sources)

    # 3) จับคู่กับโฟลเดอร์ grader แล้วเลือกไฟล์ที่จะคอมไพล์ด้วยกัน
    need = (not have_main) or opened is not None
    paired, paired_by, pwarn = pick_grader_dir(user_src, gdirs, opened, pdir, need)
    warnings += pwarn

    if gdirs and user_has_main and same_bytes(user_src, TEMPLATE_PATH):
        raise Fail(
            f"{os.path.basename(user_src)} ยังเหมือน template.cpp เป๊ะ — ยังไม่ได้เขียนอะไรเลย\n"
            f"   โจทย์ข้อนี้เป็นโจทย์ฟังก์ชัน (grader อยู่ที่ {rel(paired or gdirs[0])}) "
            "ให้เขียนฟังก์ชันตาม .h ของ grader แล้วลบ main ในไฟล์เราออก"
        )

    from_grader = False
    if have_main:
        # main มาจากไหนก็ตาม (ไฟล์เราเอง หรือไฟล์ --with) → ไม่ต้องยืม main ของ grader
        if paired and user_has_main:
            drivers = [f for f in listdir_files(paired)
                       if f.lower().endswith(".cpp") and has_main(os.path.join(paired, f))]
            if drivers:
                warnings.append(
                    f"โจทย์ข้อนี้มี grader ({rel(paired)}) แต่ {os.path.basename(user_src)} "
                    "มี main อยู่แล้ว — จะคอมไพล์ไฟล์เดียว ถ้าเป็นโจทย์ฟังก์ชันให้ลบ main ออก"
                )
    else:
        if paired is None:
            raise _no_grader_error(user_src, pdir)
        drivers = [os.path.join(paired, f) for f in listdir_files(paired)
                   if f.lower().endswith(".cpp")
                   and stem(f) != stem(user_src)   # กัน stub สำเนาใน grader ชนสัญลักษณ์
                   and has_main(os.path.join(paired, f))]
        if len(drivers) == 1:
            sources.append(drivers[0])
            from_grader = True
            notes.append(f"โจทย์ฟังก์ชัน — คอมไพล์ {rel(drivers[0])} ให้ด้วย")
        elif not drivers:
            others = [p for p in sources_with_main(pdir, skip=[os.path.abspath(paired)])
                      if os.path.abspath(p) != os.path.abspath(user_src)]
            lines = [f"{rel(paired)} ไม่มี .cpp ที่มี main — เชื่อมกับไฟล์เราไม่ได้",
                     f"   {os.path.basename(user_src)} ก็ยังไม่มี main (โจทย์ข้อนี้ต้องมีให้ครบสองฝั่ง)"]
            if others:
                lines.append(f'     • ไฟล์ที่มี main อยู่ที่อื่น: "{rel(others[0])}"')
            lines.append(f"     • เขียน main ใน {os.path.basename(user_src)} แล้วรันใหม่")
            lines.append('     • คอมไพล์ไฟล์อื่นเพิ่มด้วย: --with "<ไฟล์ที่มี main>"')
            raise Fail("\n".join(lines))
        else:
            names = " · ".join(os.path.basename(d) for d in drivers)
            raise Fail(
                f"{rel(paired)} มี .cpp ที่มี main หลายไฟล์ — เลือกให้ไม่ได้ว่าจะเชื่อมอันไหน: {names}\n"
                "   ปกติต้องมี grader.cpp อันเดียว ดู compile*.sh ว่าที่โจทย์ให้มาสั่งคอมไพล์อันไหน\n"
                '     • ระบุเอง: python3 tools/run.py --file "<ไฟล์ของเรา>" --with "<ไฟล์ที่มี main>"'
            )

    includes, iwarn = resolve_includes(sources, pdir, paired)
    warnings += iwarn

    # ทรงโจทย์ดูจากว่ามี main ของ grader มาเชื่อมไหม — ไม่ใช่จำนวนไฟล์ (--with หลายไฟล์
    # ไม่ได้แปลว่าเป็นโจทย์ฟังก์ชัน)
    kind = "function" if from_grader else "single"
    run_dir = os.path.join(pdir, RUN_DIRNAME)
    return Plan(
        pdir=pdir,
        user_src=user_src,
        kind=kind,
        sources=sources,
        includes=includes,
        std=args.std or detect_std(pdir, paired, gdirs),
        samples=collect_samples(pdir, paired, gdirs),
        run_dir=run_dir,
        binary=os.path.join(run_dir, BIN_NAME + EXE_SUFFIX),
        grader_dir=paired,
        paired_by=paired_by,
        picked_by=picked_by,
        notes=notes,
        warnings=warnings,
    )


# ---------------------------------------------------------------- คอมไพล์ / รัน
def build(plan: Plan, args: argparse.Namespace) -> None:
    if args.static and args.asan:
        raise Fail("--static กับ --asan ใช้ด้วยกันไม่ได้ — เลือกอย่างเดียว")
    cmd = [args.cxx, f"-std={plan.std}", "-O2", "-g", "-Wall", "-Wextra"]
    if args.static:
        cmd.append("-static")
    if args.asan:
        if IS_WINDOWS:
            print("⚠️  --asan บน Windows/MinGW มักใช้ไม่ได้ — ถ้าลิงก์ไม่ผ่านให้ตัด flag นี้ออก")
        cmd += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
    for inc in plan.includes:
        cmd += ["-I", inc]
    cmd += plan.sources + ["-o", plan.binary]
    if args.flags:
        cmd += args.flags.split()

    os.makedirs(plan.run_dir, exist_ok=True)
    print("$ " + " ".join(shlex.quote(c) for c in cmd))
    t0 = time.perf_counter()
    rc = subprocess.run(cmd, cwd=plan.pdir).returncode
    if rc != 0:
        raise Fail("คอมไพล์ไม่ผ่าน — อ่าน error ข้างบน (VS Code จะโชว์ใน Problems ด้วย)")
    print(f"✅ คอมไพล์ผ่าน ({time.perf_counter() - t0:.1f} s) → {rel(plan.binary)}")


def execute(plan: Plan, args: argparse.Namespace, stdin_path: str | None) -> dict:
    t0 = time.perf_counter()
    if args.interactive:
        rc = subprocess.call([plan.binary], cwd=plan.pdir)
        return {"rc": rc, "out": b"", "err": b"", "secs": time.perf_counter() - t0, "timeout": False}
    fh = open(stdin_path, "rb") if stdin_path else subprocess.DEVNULL
    try:
        proc = subprocess.run(
            [plan.binary], stdin=fh, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            cwd=plan.pdir, timeout=args.timeout,
        )
        return {"rc": proc.returncode, "out": proc.stdout, "err": proc.stderr,
                "secs": time.perf_counter() - t0, "timeout": False}
    except subprocess.TimeoutExpired as e:
        return {"rc": None, "out": e.stdout or b"", "err": e.stderr or b"",
                "secs": time.perf_counter() - t0, "timeout": True}
    finally:
        if stdin_path:
            fh.close()


def dec(b: bytes) -> str:
    return (b or b"").decode("utf-8", "replace")


def compare(got: str, exp: str) -> tuple[bool, str]:
    """เทียบแบบ grader: ไม่สนช่องว่างระหว่าง/ท้ายบรรทัด"""
    a = [ln.split() for ln in got.splitlines()]
    b = [ln.split() for ln in exp.splitlines()]
    while a and not a[-1]:
        a.pop()
    while b and not b[-1]:
        b.pop()
    if a == b:
        return True, ""
    for i, (x, y) in enumerate(zip(a, b), 1):
        if x != y:
            return False, f"บรรทัดที่ {i}\n     ของเรา: {' '.join(x)}\n     ที่คาด: {' '.join(y)}"
    return False, f"จำนวนบรรทัดไม่เท่ากัน (ของเรา {len(a)} · ที่คาด {len(b)})"


def show_result(args: argparse.Namespace, res: dict, out_path: str | None) -> None:
    if res["timeout"]:
        print(f"⏱  เกิน {args.timeout:g} วินาที — ตัดจบแล้ว (น่าจะ infinite loop)")
    else:
        print(f"⏱  {res['secs']:.3f} s · exit {res['rc']}")
    if res["err"]:
        print("--- stderr ---")
        print(dec(res["err"]).rstrip())
    if res["out"]:
        print("--- output ---")
        print(dec(res["out"]).rstrip())
    else:
        print("--- output --- (ว่าง)")
    if out_path:
        os.makedirs(os.path.dirname(out_path), exist_ok=True)
        with open(out_path, "wb") as f:
            f.write(res["out"] or b"")
        print(f"💾 {rel(out_path)}")


def resolve_stdin(plan: Plan, args: argparse.Namespace) -> tuple[str | None, str]:
    """หา input ตามลำดับ — คืน (path, ข้อความประกาศ) · path = None แปลว่าไม่มี input"""
    if args.stdin:
        p = os.path.abspath(args.stdin)
        if not os.path.isfile(p):
            raise Fail(f"ไม่เจอ --stdin {args.stdin}")
        return p, ""
    if args.sample is not None:
        hit = next((s for s in plan.samples if s["index"] == args.sample), None)
        if hit is None and 0 < args.sample <= len(plan.samples):
            hit = plan.samples[args.sample - 1]
        if hit is None:
            names = ", ".join(s["name"] for s in plan.samples) or "ไม่มีเลย"
            raise Fail(f"ไม่มี sample ที่ {args.sample} (ที่มี: {names})")
        return hit["in"], f"ℹ️  input: sample {hit['name']}"

    # ไฟล์เดียวใช้ร่วมทุกข้อ → พิมพ์บอกทุกครั้งว่ากำลังใช้ไฟล์ไหน (กันลืมเปลี่ยน input)
    if os.path.isfile(DEFAULT_STDIN):
        return DEFAULT_STDIN, f"ℹ️  input: {rel(DEFAULT_STDIN)}"

    legacy = os.path.join(plan.run_dir, INPUT_NAME)
    if os.path.isfile(legacy):
        return legacy, (f"ℹ️  input: {rel(legacy)} (ที่อยู่เก่า) — "
                        f"ย้ายมาไว้ที่ {rel(DEFAULT_STDIN)} ได้เลย")

    if plan.samples:
        s = plan.samples[0]
        return s["in"], f"ℹ️  ยังไม่มี {rel(DEFAULT_STDIN)} — ใช้ sample แรกให้ก่อน ({s['name']})"

    # ไม่มีอะไรเลย → สร้างไฟล์เปล่าให้เลย จะได้เปิดแท็บแล้ววาง input ได้ทันที
    try:
        os.makedirs(os.path.dirname(DEFAULT_STDIN), exist_ok=True)
        with open(DEFAULT_STDIN, "w", encoding="utf-8"):
            pass
        made = f"สร้าง {rel(DEFAULT_STDIN)} เปล่าไว้ให้แล้ว — วาง input ที่นั่นแล้วรันใหม่"
    except OSError:
        made = f"วาง input ที่ {rel(DEFAULT_STDIN)} แล้วรันใหม่"
    return None, f"ℹ️  ไม่มี input — {made} (หรือใช้ --sample/--stdin/-i)"


# ---------------------------------------------------------------- main
def main() -> int:
    for stream in (sys.stdout, sys.stderr):  # กัน Thai เพี้ยนบน terminal Windows
        try:
            # line_buffering: ข้อความของเราต้องออก "ก่อน" คำเตือน/error ของ g++
            # ไม่งั้น stdout ถูก buffer ไว้แล้วไปโผล่ทีหลัง (สลับที่กัน อ่านยาก)
            stream.reconfigure(encoding="utf-8", errors="replace", line_buffering=True)
        except (AttributeError, ValueError):
            pass

    argv = sys.argv[1:]
    extra: list[str] = []
    if "--" in argv:
        i = argv.index("--")
        argv, extra = argv[:i], argv[i + 1:]

    ap = argparse.ArgumentParser(
        prog="run.py",
        description="คอมไพล์+รันโจทย์ C++ (หาโฟลเดอร์ grader จากเนื้อใน ไม่ใช่จากชื่อ)",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("target", nargs="?", help="รหัสโจทย์ (C1T43) · ชื่อโฟลเดอร์ · path ไฟล์ .cpp")
    ap.add_argument("--file", default="", help="ไฟล์ที่เราแก้ (tasks.json ส่ง ${file} มา)")
    ap.add_argument("--here", action="store_true", help="ใช้โฟลเดอร์โจทย์ที่ครอบ cwd อยู่")
    ap.add_argument("-b", "--build-only", action="store_true", help="คอมไพล์อย่างเดียว ไม่รัน")
    ap.add_argument("-r", "--run-only", action="store_true", help="รัน binary เดิม ไม่คอมไพล์ใหม่")
    ap.add_argument("--sample", type=int, metavar="N", help="รันด้วย sample ที่ N")
    ap.add_argument("--all-samples", action="store_true", help="รันทุก sample แล้วสรุป")
    ap.add_argument("--stdin", metavar="FILE", help=f"ป้อนไฟล์นี้ทาง stdin (default: {INPUT_NAME} ที่ราก repo)")
    ap.add_argument("--out", metavar="FILE", help=f"เขียน stdout ลงไฟล์นี้ (default: {OUTPUT_NAME} ที่ราก repo)")
    ap.add_argument("-i", "--interactive", action="store_true", help="พิมพ์ input เองจาก terminal")
    ap.add_argument("--timeout", type=float, default=DEFAULT_TIMEOUT, help="ตัดจบใน N วินาที (default 10)")
    ap.add_argument("--std", default=None, help="มาตรฐานภาษา เช่น c++20 (default: อ่านจาก compile*.sh)")
    ap.add_argument("--static", action="store_true", help="ลิงก์แบบ static (แบบที่ grader ใช้)")
    ap.add_argument("--asan", action="store_true", help="เปิด AddressSanitizer + UBSan")
    ap.add_argument("--flags", default="", help="flag เพิ่มเติม ส่งต่อให้ g++ ตรง ๆ")
    ap.add_argument("--with", dest="with_", action="append", default=[], metavar="FILE",
                    help="คอมไพล์ไฟล์นี้เพิ่ม (ไฟล์ที่มี main ของเราเอง) — ใส่ซ้ำได้")
    ap.add_argument("-v", "--verbose", action="store_true", help="โชว์คำสั่ง g++ ที่ใช้จริง")
    ap.add_argument("--list", action="store_true", help="โชว์ layout/ไฟล์/sample แล้วจบ")
    ap.add_argument("--clean", action="store_true", help="ลบ .run/ ของข้อนี้แล้วจบ")
    ap.add_argument("--cxx", default=os.environ.get("CXX", "g++"), help="คอมไพเลอร์ (default: g++)")
    args = ap.parse_args(argv)

    try:
        plan = make_plan(args)
    except Fail as e:
        print(f"❌ {e}", file=sys.stderr)
        return 2

    # --clean จบเร็ว ไม่ต้องคอมไพล์ · แตะแค่ .run/ — input.in/output.txt ไม่หาย
    if args.clean:
        if os.path.isdir(plan.run_dir):
            shutil.rmtree(plan.run_dir)
            print(f"🧹 ลบ {rel(plan.run_dir)}")
        else:
            print(f"ไม่มี {rel(plan.run_dir)} อยู่แล้ว")
        if os.path.isfile(DEFAULT_STDIN) or os.path.isfile(DEFAULT_STDOUT):
            print(f"   · {INPUT_NAME} / {OUTPUT_NAME} ที่ราก repo ไม่ถูกแตะ")
        return 0

    if args.list or args.verbose:
        kind_th = "โจทย์ฟังก์ชัน (grader ให้ main มา)" if plan.kind == "function" else "ไฟล์เดียว"
        print(f"🎯 {rel(plan.pdir)}")
        print(f"   ทรง: {kind_th}")
        print(f"   ไฟล์เรา: {rel(plan.user_src)}  (std={plan.std})")
        print(f"   เลือกเพราะ: {plan.picked_by}")
        if plan.grader_dir:
            print(f"   grader: {rel(plan.grader_dir)}")
            if plan.paired_by:
                print(f"   จับคู่เพราะ: {plan.paired_by}")
        for n in plan.notes:
            print(f"   · {n}")
        for w in plan.warnings:
            print(f"   ⚠️  {w}")
        for inc in plan.includes:
            print(f"   -I: {rel(inc)}")
        if not plan.samples:
            print("   sample: ไม่มี")
        for s in plan.samples:
            tag = "มี .out ให้เทียบ" if s["out"] else "ไม่มี .out ให้เทียบ"
            print(f"   sample {s['name']}: {rel(s['in'])} ({tag})")
        print(f"   input: {rel(DEFAULT_STDIN)}  ·  output: {rel(DEFAULT_STDOUT)}  (ราก repo)")
        if args.list:
            return 0

    for w in plan.warnings:
        print(f"⚠️  {w}")

    if not args.run_only:
        try:
            build(plan, args)
        except Fail as e:
            print(f"❌ {e}", file=sys.stderr)
            return 1
    elif not os.path.isfile(plan.binary):
        print(f"❌ ยังไม่มี {rel(plan.binary)} — ยังไม่เคยคอมไพล์ (ตัด --run-only ออก)", file=sys.stderr)
        return 1

    if args.build_only:
        return 0

    if extra:
        print("ℹ️  argument ที่ส่งให้โปรแกรม: " + " ".join(extra))

    # ---- โหมดวนทุก sample
    if args.all_samples:
        if not plan.samples:
            print(f"❌ ไม่เจอ sample — วางไว้ที่ sample/*.in หรือ testcases/*.in หรือใช้ --stdin",
                  file=sys.stderr)
            return 2
        passed = compared = 0
        for s in plan.samples:
            print(f"\n▶️  {s['name']}  ({rel(s['in'])})")
            res = execute(plan, args, s["in"])
            got = dec(res["out"])
            print(f"   ⏱ {res['secs']:.3f} s · exit {res['rc']}"
                  + ("  ⏱ เกินเวลา!" if res["timeout"] else ""))
            if got.strip():
                print("   " + got.rstrip().replace("\n", "\n   "))
            if res["err"]:
                print("   stderr: " + dec(res["err"]).strip())
            if s["out"]:
                compared += 1
                ok, why = compare(got, read_text(s["out"]))
                if ok:
                    passed += 1
                    print(f"   ✅ ตรงกับ {os.path.basename(s['out'])}")
                else:
                    got_path = os.path.join(plan.run_dir, s["name"] + ".got")
                    os.makedirs(plan.run_dir, exist_ok=True)
                    with open(got_path, "w", encoding="utf-8") as f:
                        f.write(got)
                    print(f"   ❌ ไม่ตรง: {why}\n      เขียนของเราที่ {rel(got_path)}")
            else:
                print("   ⚠️  ไม่มี .out ให้เทียบ")
        print(f"\n📊 เทียบได้ {compared} · ผ่าน {passed}"
              + (f" · ไม่มี .out {len(plan.samples) - compared}" if compared < len(plan.samples) else ""))
        return 0 if passed == compared else 1

    # ---- โหมดรันครั้งเดียว
    try:
        stdin_path, announce = resolve_stdin(plan, args)
    except Fail as e:
        print(f"❌ {e}", file=sys.stderr)
        return 2
    if announce:
        print(announce)

    out_path = os.path.abspath(args.out) if args.out else DEFAULT_STDOUT
    res = execute(plan, args, stdin_path)
    show_result(args, res, None if args.interactive else out_path)
    return 0 if (res["rc"] == 0 and not res["timeout"]) else 1


if __name__ == "__main__":
    raise SystemExit(main())
