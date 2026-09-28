#!/usr/bin/env python3
"""ย้ายโจทย์ที่ได้เต็มเข้า _done/ แล้วสร้างตารางคะแนนใน README.md ใหม่

วิธีใช้ (รันจากที่ไหนก็ได้):
    python3 tools/sync_done.py

ขั้นตอน: ก็อปตารางคะแนนจากหน้า grader มาวางทับ tools/score.txt ก่อน
score.txt คือความจริง — ตารางใน README สร้างจากไฟล์นั้น ห้ามแก้มือ
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import (  # noqa: E402
    DONE_DIRNAME,
    README_PATH,
    REPO_ROOT,
    SCORE_END,
    SCORE_START,
    find_task,
    done_dir_for,
    parse_score,
)


def move_into_done(task_id: str, src: str, grader_dir: str) -> bool:
    """ย้ายโฟลเดอร์โจทย์เข้า _done/ — ใช้ git mv ถ้าไฟล์ถูก track อยู่"""
    dst = os.path.join(done_dir_for(grader_dir), task_id)
    if os.path.exists(dst):
        print(f"   ⚠️  {task_id}: มีอยู่ใน _done/ แล้ว — ข้ามการย้าย")
        return False

    rel_src = os.path.relpath(src, REPO_ROOT)
    rel_dst = os.path.relpath(dst, REPO_ROOT)

    result = subprocess.run(
        ["git", "mv", rel_src, rel_dst],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        # ยังไม่ถูก track (หรือไม่ใช่ git repo) — ย้ายด้วยมือเอา
        shutil.move(src, dst)

    print(f"   📦 ย้าย {task_id} → {os.path.relpath(dst, REPO_ROOT)}")
    return True


def render_table(tasks) -> str:
    full = sum(1 for t in tasks if t.is_full)
    lines = [
        f"ได้เต็ม **{full} / {len(tasks)}** ข้อ\n",
        "| สถานะ | คะแนน | Task | ชื่อโจทย์ | ลิมิต | ไฟล์ |",
        "| :---: | :---: | :--- | :--- | :---: | :---: |",
    ]

    for t in tasks:
        found = find_task(t.task_id)
        if found:
            link = f"[📁]({os.path.relpath(found[1], REPO_ROOT)})"
        else:
            link = "—"

        limit = f"{t.time_limit} / {t.mem_limit}" if t.time_limit else ""
        name = t.name or "—"
        lines.append(
            f"| {t.status} | {t.score} / {t.max_score} | **{t.task_id}** | {name} | {limit} | {link} |"
        )

    return "\n".join(lines)


def write_readme(table: str) -> bool:
    with open(README_PATH, encoding="utf-8") as f:
        content = f.read()

    if SCORE_START not in content or SCORE_END not in content:
        print(f"❌ หา marker {SCORE_START} / {SCORE_END} ใน README.md ไม่เจอ — ข้าม")
        return False

    head, rest = content.split(SCORE_START, 1)
    _, tail = rest.split(SCORE_END, 1)
    new_content = f"{head}{SCORE_START}\n{table}\n{SCORE_END}{tail}"

    if new_content == content:
        return False

    with open(README_PATH, "w", encoding="utf-8") as f:
        f.write(new_content)
    return True


def main() -> int:
    try:
        tasks = parse_score()
    except FileNotFoundError as e:
        print(f"❌ {e}")
        return 1

    if not tasks:
        print("❌ อ่าน score.txt ได้ 0 บรรทัด — เช็คว่าวางตารางคะแนนมาถูกไฟล์มั้ย")
        return 1

    print(f"📊 อ่านได้ {len(tasks)} ข้อ จาก tools/score.txt")
    print("-" * 50)

    moved = 0
    for t in tasks:
        if not t.is_full:
            continue

        found = find_task(t.task_id)
        if found is None:
            continue

        grader, path, in_done = found
        if in_done:
            continue

        grader_dir = os.path.dirname(path)
        print(f"🟢 {t.task_id} — {t.name}")
        if move_into_done(t.task_id, path, grader_dir):
            moved += 1

    print("-" * 50)
    print(f"📦 ย้ายเข้า _done/ ทั้งหมด {moved} ข้อ")

    if write_readme(render_table(tasks)):
        print("📝 อัปเดตตารางคะแนนใน README.md แล้ว")
    else:
        print("😎 ตารางคะแนนใน README.md เป็นปัจจุบันอยู่แล้ว")

    missing = [t.task_id for t in tasks if find_task(t.task_id) is None]
    if missing:
        print("-" * 50)
        print(f"⚠️  ยังไม่มีโฟลเดอร์ {len(missing)} ข้อ (ดึงด้วย tools/fetch_statements.fish):")
        for task_id in missing:
            print(f"   - {task_id}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
