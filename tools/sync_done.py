#!/usr/bin/env python3
"""ย้ายโจทย์ที่ได้เต็มเข้า _done/ แล้วสร้างตารางคะแนนใน README.md ใหม่

วิธีใช้ (รันจากที่ไหนก็ได้):
    python3 tools/sync_done.py

ขั้นตอน: ก็อปตารางคะแนนจากหน้าเว็บของ grader มาวางทับ tools/scores/<ชื่อ>.txt
ไฟล์พวกนั้นคือความจริง — ตารางใน README สร้างจากไฟล์เหล่านั้น ห้ามแก้มือ
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import (  # noqa: E402
    README_PATH,
    REPO_ROOT,
    SCORES_DIR,
    SCORE_END,
    SCORE_START,
    done_dir_for,
    find_task,
    parse_all_scores,
    reset_index,
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


def render_group(grader, tasks) -> str:
    """ตารางคะแนนของ grader หนึ่งตัว"""
    full = sum(1 for t in tasks if t.is_full)
    lines = [
        f"### {grader.title} — ได้เต็ม **{full} / {len(tasks)}** ข้อ\n",
        "| สถานะ | คะแนน | Task | ชื่อโจทย์ | ลิมิต | ไฟล์ |",
        "| :---: | :---: | :--- | :--- | :---: | :---: |",
    ]

    for t in tasks:
        found = find_task(t.task_id)
        link = f"[📁]({os.path.relpath(found[1], REPO_ROOT)})" if found else "—"
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
    groups = parse_all_scores()

    if not groups:
        rel = os.path.relpath(SCORES_DIR, REPO_ROOT)
        print(f"❌ ยังไม่มีไฟล์คะแนนใน {rel}/ ที่อ่านได้")
        print("   เปิดหน้าเว็บ grader → ก็อปตารางคะแนน → วางในไฟล์ชื่อ <ชื่อ contest>.txt")
        print("   ดูรายชื่อ contest ทั้งหมดได้ที่ tools/graders.txt")
        return 1

    total = sum(len(tasks) for _, tasks in groups)
    print(f"📊 อ่านได้ {total} ข้อ จาก {len(groups)} grader")
    print("-" * 50)

    moved = 0
    for grader, tasks in groups:
        full_tasks = [t for t in tasks if t.is_full]
        for t in full_tasks:
            found = find_task(t.task_id)
            if found is None or found[2]:  # ไม่มีโฟลเดอร์ หรืออยู่ใน _done แล้ว
                continue
            print(f"🟢 [{grader.title}] {t.task_id} — {t.name}")
            if move_into_done(t.task_id, found[1], os.path.dirname(found[1])):
                moved += 1

    print("-" * 50)
    print(f"📦 ย้ายเข้า _done/ ทั้งหมด {moved} ข้อ")

    reset_index()  # เพิ่งย้ายโฟลเดอร์ไป — path ที่แคชไว้ใช้ไม่ได้แล้ว
    table = "\n\n".join(render_group(g, tasks) for g, tasks in groups)
    if write_readme(table):
        print("📝 อัปเดตตารางคะแนนใน README.md แล้ว")
    else:
        print("😎 ตารางคะแนนใน README.md เป็นปัจจุบันอยู่แล้ว")

    missing = [(g, t) for g, tasks in groups for t in tasks if find_task(t.task_id) is None]
    if missing:
        print("-" * 50)
        print(f"⚠️  ยังไม่มีโฟลเดอร์ {len(missing)} ข้อ — โหลดมือจากหน้าเว็บของ contest:")
        by_grader: dict[str, list[str]] = {}
        for grader, t in missing:
            by_grader.setdefault(grader.title, []).append(t.task_id)
        for title, ids in by_grader.items():
            print(f"   [{title}] {len(ids)} ข้อ: {' '.join(ids)}")
        print()
        print("   โหลด PDF ลงโฟลเดอร์เดียวกันให้ครบ แล้วจัดเข้าโฟลเดอร์ด้วย:")
        print("      fish tools/organize.fish <โฟลเดอร์ที่โหลดมา> --cpp")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
