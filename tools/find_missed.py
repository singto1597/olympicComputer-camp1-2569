#!/usr/bin/env python3
"""เช็คว่าโจทย์ข้อไหนใน tools/score.txt ที่ยังไม่มีโฟลเดอร์ในเครื่อง

วิธีใช้:
    python3 tools/find_missed.py

ต่างจาก sync_done.py ตรงที่ตัวนี้**ไม่แก้อะไรเลย** แค่อ่านแล้วรายงาน
"""

from __future__ import annotations

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import find_task, parse_score  # noqa: E402


def main() -> int:
    try:
        tasks = parse_score()
    except FileNotFoundError as e:
        print(f"❌ {e}")
        return 1

    missing = [t for t in tasks if find_task(t.task_id) is None]

    print(f"🔍 เช็ค {len(tasks)} ข้อ จาก tools/score.txt")
    print("-" * 50)

    if not missing:
        print("✨ ครบถ้วน! ทุกข้อมีโฟลเดอร์ในเครื่องแล้ว")
        return 0

    for t in missing:
        print(f"⚠️  {t.task_id:<12} {t.name}")

    print("-" * 50)
    print(f"รวม {len(missing)} ข้อที่ยังไม่มีโฟลเดอร์")
    print("\nดึงมาทีเดียวด้วย:")
    print("   fish tools/fetch_statements.fish " + " ".join(t.task_id for t in missing[:8]))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
