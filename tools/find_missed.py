#!/usr/bin/env python3
"""เช็คว่าโจทย์ข้อไหนใน tools/scores/*.txt ที่ยังไม่มีโฟลเดอร์ในเครื่อง

วิธีใช้:
    python3 tools/find_missed.py

ต่างจาก sync_done.py ตรงที่ตัวนี้**ไม่แก้อะไรเลย** แค่อ่านแล้วรายงาน
"""

from __future__ import annotations

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import find_task, parse_all_scores  # noqa: E402


def main() -> int:
    groups = parse_all_scores()

    if not groups:
        print("❌ ยังไม่มีไฟล์คะแนนใน tools/scores/ ที่อ่านได้ — ดูรายชื่อที่ tools/graders.txt")
        return 1

    total = sum(len(tasks) for _, tasks in groups)
    print(f"🔍 เช็ค {total} ข้อ จาก {len(groups)} grader")
    print("-" * 50)

    any_missing = False
    for grader, tasks in groups:
        missing = [t for t in tasks if find_task(t.task_id) is None]
        if not missing:
            print(f"✨ [{grader.title}] ครบถ้วน — ทุกข้อมีโฟลเดอร์แล้ว")
            continue

        any_missing = True
        print(f"⚠️  [{grader.title}] ขาด {len(missing)} ข้อ")
        for t in missing:
            print(f"     {t.task_id:<12} {t.name}")

    print("-" * 50)
    if not any_missing:
        print("✨ ครบถ้วนทั้งหมด!")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
