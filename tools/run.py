#!/usr/bin/env python3
"""คอมไพล์ + รันโจทย์ C++ ในเครื่อง — สคริปต์กลางตัวเดียว ทั้ง terminal และ VS Code task

โจทย์ใน repo นี้มี 2 ทรง และสคริปต์นี้รู้จักทั้งคู่

  1. โจทย์แบบฟังก์ชัน — grader ให้ `provided/grader.cpp` (มี main) + `provided/<ID>.h`
     มา เราเขียนแค่ตัวฟังก์ชันในไฟล์ที่วางชั้นบนสุดของโฟลเดอร์โจทย์ ชื่อตามคอลัมน์
     Files (บางข้อไม่ตรงกับรหัส Task เช่น C1C02 → `exchange.cpp`)
  2. โจทย์ไฟล์เดียว — `<ID>.cpp` มี main ในตัว

ตัวอย่าง
  python3 tools/run.py C1T43                  # หาโฟลเดอร์จากรหัส แล้วคอมไพล์ + รัน
  python3 tools/run.py C1T43 -b               # คอมไพล์อย่างเดียว
  python3 tools/run.py C1T43 --sample 1       # รันด้วย sample ที่ 1 (+เทียบ .out ถ้ามี)
  python3 tools/run.py C1T43 --all-samples    # วนทุก sample แล้วสรุปผ่านกี่อัน
  python3 tools/run.py C1T43 --stdin in.txt --out res.txt
  python3 tools/run.py --file <path>/C1T43.cpp   # วิธีที่ tasks.json ใช้ (${file})
  python3 tools/run.py --here                 # ใช้โฟลเดอร์โจทย์ที่ครอบ cwd อยู่
  python3 tools/run.py C1T38 --with local/C1T38_main.cpp
  python3 tools/run.py C1T43 --asan           # AddressSanitizer + UBSan (หาล้น/oob)
  python3 tools/run.py C1T43 -- 2 3           # ส่ง argument ที่เหลือให้โปรแกรม
  python3 tools/run.py C1T43 --list           # โชว์ layout/ไฟล์/sample แล้วจบ
  python3 tools/run.py C1T43 --clean          # ลบ .run/ ของข้อนี้

ของที่ออกมา (อยู่ใต้ .run/ ของ "โฟลเดอร์โจทย์" ไม่ใช่ราก repo)
  .run/bin          โปรแกรมที่คอมไพล์แล้ว (.exe บน Windows)
  .run/output.txt   stdout ของครั้งล่าสุด
  .run/<sample>.got ผลของ sample ที่เทียบไม่ผ่าน
  .run/input.in     วาง input ไว้ที่นี่ได้ แล้วรัน `run.py C1T43` เฉย ๆ
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

# ของที่ grader ให้มา — เราไม่แก้ ("attachment" = ชื่อเดิมก่อน migrate ยังรองรับ)
PROVIDED_NAMES = ("provided", "attachment")
# ที่ที่ sample/testcase อาจอยู่ — เจอที่แรกชนะ
SAMPLE_DIRS = ("testcases", "provided/sample", "sample")
EXPECTED_EXTS = (".out", ".ans")
# โฟลเดอร์ที่ไม่ใช่โฟลเดอร์โจทย์ — ใช้ทั้งตอนไล่ขึ้นหาว่าโฟลเดอร์โจทย์คืออันไหน
# และตอน folder_names.py ตัดสินใจว่าจะรายงานอะไร (ดู SUPPORT_DIRNAMES ที่นั่น)
SUPPORT_DIRNAMES = {"provided", "attachment", "sample", "testcases", "local"}

DEFAULT_STD = "c++17"
DEFAULT_TIMEOUT = 10.0

# ไฟล์ซ้อม/เวอร์ชันลอง — ไม่ใช่ไฟล์ที่ส่ง
VARIANT_RE = re.compile(r"_(?:singlefile|localtest\w*|af|TA-Teaching|optimize)$", re.I)
MAIN_RE = re.compile(r"^[^\S\n]*(?:int|void|signed)\s+main\s*\(", re.M)
STD_RE = re.compile(r"-std=(gnu\+\+|c\+\+)(\d+)")
NUM_RE = re.compile(r"(\d+)")


class Fail(Exception):
    """ปัญหาที่อธิบายให้ผู้ใช้เข้าใจได้ — พิมพ์ข้อความแล้วจบ ไม่ต้อง traceback"""


# ---------------------------------------------------------------- ยูทิลิตี้
def rel(path: str) -> str:
    try:
        return os.path.relpath(path, REPO_ROOT)
    except ValueError:  # คนละ drive บน Windows
        return path


def read_text(path: str) -> str:
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            return f.read()
    except OSError:
        return ""


def has_main(path: str) -> bool:
    return bool(MAIN_RE.search(read_text(path)))


def in_support_dir(path: str) -> bool:
    """ไฟล์นี้อยู่ในโฟลเดอร์เสริม (provided/ attachment/ ...) หรือเปล่า"""
    parts = os.path.abspath(path).replace("\\", "/").split("/")
    return any(p in SUPPORT_DIRNAMES for p in parts)


def listdir_files(d: str) -> list[str]:
    try:
        return sorted(n for n in os.listdir(d) if os.path.isfile(os.path.join(d, n)))
    except OSError:
        return []


def provided_dir(pdir: str) -> str | None:
    for name in PROVIDED_NAMES:
        d = os.path.join(pdir, name)
        if os.path.isdir(d):
            return d
    return None


def user_sources(pdir: str) -> list[str]:
    """ไฟล์ .cpp ที่เป็นของเรา = ชั้นบนสุดของโฟลเดอร์โจทย์ (ของ grader อยู่ใน provided/)"""
    return [os.path.join(pdir, n) for n in listdir_files(pdir) if n.lower().endswith(".cpp")]


# ---------------------------------------------------------------- หาโฟลเดอร์โจทย์
def looks_like_problem_dir(d: str) -> bool:
    if not os.path.isdir(d) or os.path.basename(d) in SUPPORT_DIRNAMES:
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
    """ไล่จากไฟล์/โฟลเดอร์ขึ้นไปจนเจอโฟลเดอร์โจทย์ (ข้าม provided/ local/ ...)"""
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
        dirs[:] = sorted(d for d in dirs if not d.startswith(".") and d not in SUPPORT_DIRNAMES)
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
        "   · หรือส่ง path ไฟล์ก็ได้: python3 tools/run.py --file <path>.cpp"
    )


def pick_user_source(pdir: str, explicit: str) -> str:
    """ไฟล์ที่เราแก้ — ค่าเริ่มต้น = ไฟล์ .cpp ชั้นบนสุดที่ตรงรหัสโจทย์"""
    if explicit:
        p = os.path.abspath(explicit)
        if not os.path.isfile(p):
            raise Fail(f"ไม่เจอไฟล์ {explicit}")
        return p

    cands = user_sources(pdir)
    if not cands:
        raise Fail(
            f"ไม่เจอไฟล์ .cpp ใน {rel(pdir)}\n"
            "   ไฟล์ที่เราแก้ต้องอยู่ชั้นบนสุดของโฟลเดอร์โจทย์ (ของ grader อยู่ใน provided/)"
        )
    tid = task_id_from_folder(os.path.basename(pdir))
    for p in cands:
        if os.path.splitext(os.path.basename(p))[0] == tid:
            return p
    if len(cands) == 1:
        return cands[0]
    prim = [p for p in cands if not VARIANT_RE.search(os.path.splitext(os.path.basename(p))[0])]
    if len(prim) == 1:
        return prim[0]
    names = " · ".join(os.path.basename(p) for p in cands)
    raise Fail(f"เลือกไฟล์ไม่ได้ — มีหลายไฟล์: {names}\n   ระบุให้ชัดด้วย --file <ชื่อไฟล์>")


# ---------------------------------------------------------------- sample
def find_sample_dir(pdir: str) -> str | None:
    for r in SAMPLE_DIRS:
        d = os.path.join(pdir, *r.split("/"))
        if os.path.isdir(d) and glob.glob(os.path.join(d, "*.in")):
            return d
    return None


def collect_samples(pdir: str) -> list[dict]:
    d = find_sample_dir(pdir)
    if not d:
        return []
    out = []
    for n in sorted(glob.glob(os.path.join(d, "*.in"))):
        stem = os.path.basename(n)[:-3]
        exp = None
        for ext in EXPECTED_EXTS:
            cand = os.path.join(d, stem + ext)
            if os.path.isfile(cand):
                exp = cand
                break
        m = NUM_RE.search(stem)
        out.append({"name": stem, "index": int(m.group(1)) if m else None, "in": n, "out": exp})
    return out


def detect_std(pdir: str) -> str:
    """อ่าน -std= จากสคริปต์คอมไพล์ที่ grader ให้มา (AplusB/Georgette = gnu++20)"""
    for d in (provided_dir(pdir), pdir):
        if not d:
            continue
        for n in listdir_files(d):
            if n.startswith(("compile", "build", "run")) and n.endswith((".sh", ".bat")):
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
    notes: list[str] = field(default_factory=list)


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

    # 2) เลือกไฟล์ที่เราแก้ (ไฟล์ใน provided/ = เบาะแสหาโฟลเดอร์เท่านั้น)
    explicit = hint if (hint.lower().endswith(".cpp") and not in_support_dir(hint)) else ""
    user_src = pick_user_source(pdir, explicit)

    # 3) ประกอบคำสั่งคอมไพล์
    pd = provided_dir(pdir)
    includes = [pd] if pd else []
    sources = [user_src]
    notes: list[str] = []

    if not has_main(user_src) and pd:
        # โจทย์ฟังก์ชัน — ต้องมี grader ที่มี main มาเชื่อมด้วย
        stem = os.path.splitext(os.path.basename(user_src))[0]
        graders = [
            os.path.join(pd, n)
            for n in listdir_files(pd)
            if n.lower().endswith(".cpp")
            and os.path.splitext(n)[0] != stem   # กัน stub สำเนาใน attachment ชนสัญลักษณ์
            and has_main(os.path.join(pd, n))
        ]
        if graders:
            sources += graders
            notes.append("โจทย์ฟังก์ชัน — คอมไพล์ " + " + ".join(rel(p) for p in graders) + " ให้ด้วย")
        else:
            notes.append("ใน provided/ ไม่มี .cpp ที่มี main — คอมไพล์เฉพาะไฟล์เรา")

    for w in args.with_:
        w = os.path.abspath(w)
        if not os.path.isfile(w):
            raise Fail(f"ไม่เจอไฟล์ --with {w}")
        sources.append(w)

    if not any(has_main(p) for p in sources):
        lines = [f"{os.path.basename(user_src)} ไม่มี main และโฟลเดอร์นี้ไม่มี grader ที่มี main ให้"]
        lines.append("   โจทย์ข้อนี้ต้องมี main เอง — เลือกทางใดทางหนึ่ง")
        lines.append(f"     • เขียน main ใน {os.path.basename(user_src)} แล้วรันใหม่")
        others = [p for p in user_sources(pdir) if p != user_src and has_main(p)]
        if others:
            lines.append(f"     • ใช้ไฟล์ที่มี main อยู่แล้ว: python3 tools/run.py --file {rel(others[0])}")
        lines.append("     • คอมไพล์ไฟล์อื่นเพิ่มด้วย: --with <ไฟล์ที่มี main>")
        raise Fail("\n".join(lines))

    kind = "function" if len(sources) > 1 else "single"
    run_dir = os.path.join(pdir, RUN_DIRNAME)
    return Plan(
        pdir=pdir,
        user_src=user_src,
        kind=kind,
        sources=sources,
        includes=includes,
        std=args.std or detect_std(pdir),
        samples=collect_samples(pdir),
        run_dir=run_dir,
        binary=os.path.join(run_dir, BIN_NAME + EXE_SUFFIX),
        notes=notes,
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
        description="คอมไพล์+รันโจทย์ C++ (โจทย์ฟังก์ชันที่มี provided/ ก็ใช้ได้)",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("target", nargs="?", help="รหัสโจทย์ (C1T43) · ชื่อโฟลเดอร์ · path ไฟล์ .cpp")
    ap.add_argument("--file", default="", help="ไฟล์ที่เราแก้ (tasks.json ส่ง ${file} มา)")
    ap.add_argument("--here", action="store_true", help="ใช้โฟลเดอร์โจทย์ที่ครอบ cwd อยู่")
    ap.add_argument("-b", "--build-only", action="store_true", help="คอมไพล์อย่างเดียว ไม่รัน")
    ap.add_argument("-r", "--run-only", action="store_true", help="รัน binary เดิม ไม่คอมไพล์ใหม่")
    ap.add_argument("--sample", type=int, metavar="N", help="รันด้วย sample ที่ N")
    ap.add_argument("--all-samples", action="store_true", help="รันทุก sample แล้วสรุป")
    ap.add_argument("--stdin", metavar="FILE", help="ป้อนไฟล์นี้ทาง stdin")
    ap.add_argument("--out", metavar="FILE", help="เขียน stdout ลงไฟล์นี้ (default .run/output.txt)")
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

    # --clean จบเร็ว ไม่ต้องคอมไพล์
    if args.clean:
        if os.path.isdir(plan.run_dir):
            shutil.rmtree(plan.run_dir)
            print(f"🧹 ลบ {rel(plan.run_dir)}")
        else:
            print(f"ไม่มี {rel(plan.run_dir)} อยู่แล้ว")
        return 0

    if args.list or args.verbose:
        kind_th = "โจทย์ฟังก์ชัน (grader ให้ main มา)" if plan.kind == "function" else "ไฟล์เดียว"
        print(f"🎯 {rel(plan.pdir)}")
        print(f"   ทรง: {kind_th}")
        print(f"   ไฟล์เรา: {rel(plan.user_src)}  (std={plan.std})")
        for n in plan.notes:
            print(f"   · {n}")
        if plan.includes:
            print(f"   -I: {rel(plan.includes[0])}")
        if not plan.samples:
            print("   sample: ไม่มี")
        for s in plan.samples:
            tag = "มี .out ให้เทียบ" if s["out"] else "ไม่มี .out ให้เทียบ"
            print(f"   sample {s['name']}: {rel(s['in'])} ({tag})")
        if args.list:
            return 0

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
            print("❌ ไม่เจอ sample — วางไว้ที่ sample/*.in หรือ testcases/*.in หรือใช้ --stdin",
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

    # ---- โหมดรันครั้งเดียว: หา input ตามลำดับ
    stdin_path, announce = None, ""
    if args.stdin:
        stdin_path = os.path.abspath(args.stdin)
        if not os.path.isfile(stdin_path):
            print(f"❌ ไม่เจอ --stdin {args.stdin}", file=sys.stderr)
            return 2
    elif args.sample is not None:
        hit = next((s for s in plan.samples if s["index"] == args.sample), None)
        if hit is None and 0 < args.sample <= len(plan.samples):
            hit = plan.samples[args.sample - 1]
        if hit is None:
            names = ", ".join(s["name"] for s in plan.samples) or "ไม่มีเลย"
            print(f"❌ ไม่มี sample ที่ {args.sample} (ที่มี: {names})", file=sys.stderr)
            return 2
        stdin_path = hit["in"]
    else:
        default_in = os.path.join(plan.run_dir, "input.in")
        if os.path.isfile(default_in):
            stdin_path = default_in
        elif plan.samples:
            stdin_path = plan.samples[0]["in"]
            announce = f"ℹ️  ยังไม่มี {rel(default_in)} — ใช้ sample แรกให้ก่อน ({plan.samples[0]['name']})"

    if announce:
        print(announce)
    if stdin_path is None:
        print(f"ℹ️  ไม่มี input — โปรแกรมจะเจอ EOF (วางไฟล์ที่ {rel(os.path.join(plan.run_dir, 'input.in'))}"
              " หรือใช้ --sample/--stdin/--interactive)")

    out_path = os.path.abspath(args.out) if args.out else os.path.join(plan.run_dir, "output.txt")
    res = execute(plan, args, stdin_path)
    show_result(args, res, None if args.interactive else out_path)
    return 0 if (res["rc"] == 0 and not res["timeout"]) else 1


if __name__ == "__main__":
    raise SystemExit(main())
