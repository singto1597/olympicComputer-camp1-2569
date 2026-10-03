#!/usr/bin/env python3
"""ตั้งชื่อโฟลเดอร์โจทย์เป็น <ลำดับ>_<รหัสโจทย์>_<ชื่อโจทย์>

วิธีใช้ (รันจากที่ไหนก็ได้):
    python3 tools/folder_names.py               # ดูเฉย ๆ ว่าอะไรจะถูกเปลี่ยนชื่อ
    python3 tools/folder_names.py --apply       # เปลี่ยนจริง (ใช้ git mv)
    python3 tools/folder_names.py --print-map   # พิมพ์ "รหัสโจทย์<TAB>ชื่อโฟลเดอร์"

ชื่อโจทย์และเลขลำดับมาจาก tools/scores/*.txt ผ่าน scorelib — ลำดับนับ 1 ใหม่
ในแต่ละ contest ตามลำดับบรรทัดในไฟล์คะแนน (ลำดับเดียวกับตารางใน README) เพื่อให้
เปิดโฟลเดอร์แล้วเรียงตามลำดับโจทย์ ไม่ใช่เรียงตามรหัส (C1PC → C1PE → C1PR)

ข้อที่ยังไม่มีไฟล์คะแนนจะคงชื่อรหัสเดิม **ไฟล์ข้างในไม่ถูกแตะ** ยังเป็น
<รหัสโจทย์>.cpp / <รหัสโจทย์>.pdf ตามที่ grader บังคับชื่อไฟล์ตอน submit
(คอลัมน์ Files) — โฟลเดอร์กับไฟล์จึงชื่อไม่เหมือนกันโดยตั้งใจ
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import (  # noqa: E402
    DONE_DIRNAME,
    PROBLEMS_DIR,
    REPO_ROOT,
    folder_name_for,
    parse_all_scores,
    task_id_from_folder,
)

PROBLEM_FILE_EXTS = (".cpp", ".c", ".pdf", ".html")


def build_map() -> dict[str, str]:
    """{รหัสโจทย์: ชื่อโฟลเดอร์ที่ควรเป็น} จากไฟล์คะแนนทุก contest

    เลขลำดับนับ 1 ใหม่ในแต่ละ contest ตามลำดับบรรทัดในไฟล์คะแนน
    (ลำดับเดียวกับตารางคะแนนใน README) — ไม่ใช่เรียงตามรหัส
    """
    names: dict[str, str] = {}
    for _grader, tasks in parse_all_scores():
        for order, t in enumerate(tasks, start=1):
            names.setdefault(t.task_id, folder_name_for(t.task_id, t.name, order))
    return names


def iter_folders():
    """เดิน problems/ ทุกโฟลเดอร์ — รวมของใน _done/ แต่ไม่รวมตัว _done เอง"""
    for root, dirs, _ in os.walk(PROBLEMS_DIR):
        dirs[:] = sorted(d for d in dirs if not d.startswith("."))
        for name in dirs:
            if name == DONE_DIRNAME:
                continue
            yield os.path.join(root, name)


def looks_like_problem_dir(path: str) -> bool:
    """มีไฟล์โจทย์อยู่ในนั้นตรง ๆ ไหม — ใช้แยกโฟลเดอร์โจทย์ออกจากโฟลเดอร์แม่"""
    try:
        entries = os.listdir(path)
    except OSError:
        return False
    return any(e.endswith(PROBLEM_FILE_EXTS) for e in entries)


def rename(src: str, dst: str) -> None:
    """git mv ถ้าไฟล์ถูก track อยู่ ไม่งั้นย้ายด้วยมือ — แบบเดียวกับ sync_done.move_into_done"""
    rel_src = os.path.relpath(src, REPO_ROOT)
    rel_dst = os.path.relpath(dst, REPO_ROOT)
    result = subprocess.run(
        ["git", "mv", rel_src, rel_dst],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        shutil.move(src, dst)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="ตั้งชื่อโฟลเดอร์โจทย์เป็น <รหัสโจทย์>_<ชื่อโจทย์> จากไฟล์คะแนน",
    )
    parser.add_argument("--apply", action="store_true", help="เปลี่ยนชื่อจริง (ไม่ใส่ = ดูเฉย ๆ)")
    parser.add_argument("--print-map", action="store_true", help="พิมพ์รหัสโจทย์<TAB>ชื่อโฟลเดอร์ แล้วจบ")
    args = parser.parse_args()

    names = build_map()
    if not names:
        print("❌ ยังไม่มีไฟล์คะแนนใน tools/scores/ ที่อ่านได้ — ดูรายชื่อที่ tools/graders.txt")
        return 1

    if args.print_map:
        for task_id in sorted(names):
            print(f"{task_id}\t{names[task_id]}")
        return 0

    todos: list[tuple[str, str]] = []
    collisions: list[tuple[str, str]] = []
    unknown: list[str] = []

    for path in iter_folders():
        name = os.path.basename(path)
        target = names.get(task_id_from_folder(name))

        if target is None:
            if looks_like_problem_dir(path):
                unknown.append(path)
            continue
        if name == target:
            continue

        dst = os.path.join(os.path.dirname(path), target)
        if os.path.exists(dst):
            collisions.append((path, dst))
            continue
        todos.append((path, dst))

    if not todos and not collisions and not unknown:
        print(f"✨ ชื่อโฟลเดอร์ตรงกับชื่อโจทย์ครบแล้ว ({len(names)} ข้อ)")
        return 0

    for src, dst in todos:
        print(f"{'✅' if args.apply else '👀'} {os.path.relpath(src, REPO_ROOT)}")
        print(f"   → {os.path.basename(dst)}")

    if args.apply:
        for src, dst in todos:
            rename(src, dst)

    print("-" * 50)
    if args.apply:
        print(f"🎉 เปลี่ยนชื่อ {len(todos)} โฟลเดอร์ (ไฟล์ข้างในไม่ได้แตะ)")
    else:
        print(f"👀 จะเปลี่ยนชื่อ {len(todos)} โฟลเดอร์ — ยังไม่แตะอะไร (ใส่ --apply เพื่อทำจริง)")

    for src, dst in collisions:
        print(f"⚠️  ข้าม {os.path.relpath(src, REPO_ROOT)} — มี {os.path.basename(dst)} อยู่แล้ว")

    for path in unknown:
        print(f"⚠️  {os.path.relpath(path, REPO_ROOT)} — รหัสไม่ตรงกับข้อไหนในไฟล์คะแนน (ข้าม)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
