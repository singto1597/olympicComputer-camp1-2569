#!/usr/bin/env python3
"""ยกโจทย์+โค้ดจากโฟลเดอร์ค่ายปีเก่า เข้ามาเป็นโฟลเดอร์โจทย์ของ repo นี้

วิธีใช้ (รันจากที่ไหนก็ได้):
    python3 tools/import_old_camp.py <โฟลเดอร์ต้นทาง> --contest camp1_1 \\
            --dest problems/2_incamp/1_first_half/cmu-grader [--dry-run] [--move]

ต้นทางที่รองรับ = โฟลเดอร์รุ่นเก่าที่แบ่งเป็นโฟลเดอร์ย่อยข้อละอัน เช่น
    1.CMU_Grader/16.C1T1/{ C1T1 (C++).pdf , main.cpp , main.exe , input.in }
แปลงเป็น
    <dest>/15_C1T1_Banana_Shop/{ C1T1.pdf , C1T1.cpp }

กติกา
  · ชื่อโฟลเดอร์/ลำดับ มาจาก tools/scores/<contest>.txt ผ่าน scorelib.folder_name_for()
    (ชื่อจะตรงกับที่ folder_names.py / organize.fish ผลิต — ไม่ประกอบชื่อเอง)
  · ไฟล์ข้างในใช้รหัสโจทย์ล้วน ๆ ตามที่ grader บังคับตอน submit
    main.cpp → <รหัส>.cpp · main_optimize.cpp → <รหัส>_optimize.cpp
    <รหัส> (...).pdf → <รหัส>.pdf (ตัดวงเล็บแบบเดียวกับ organize.fish)
  · ไฟล์คอมไพล์/รัน (main, *.exe, input.in, output.txt) ไม่เอามา — repo ใช้ .run/
  · ข้อที่ต้นทางไม่มีโค้ด จะได้ <รหัส>.cpp จาก template.cpp ไปเริ่มเขียน
  · default = คัดลอก (ต้นทางไม่ถูกแตะ) · --move = ย้ายจริง
"""

from __future__ import annotations

import argparse
import os
import re
import shutil
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import (  # noqa: E402
    REPO_ROOT,
    folder_name_for,
    get_grader,
    parse_score,
)

# โฟลเดอร์ย่อยของต้นทางหน้าตาเป็น "<เลขลำดับ>.<รหัสโจทย์>" เช่น "16.C1T1"
_SRC_DIR_RE = re.compile(r"^\d+\.(.+)$")

# ไฟล์ที่ยอมรับจากต้นทาง → ปลายทาง (None = ข้าม เพราะเป็นไฟล์คอมไพล์/รัน)
_SKIP_NAMES = {"input.in", "output.txt", "main", "a.out"}
_SKIP_EXTS = {".exe", ".out", ".o", ".obj", ".gch"}


def _pdf_stem(filename: str) -> str:
    """'C1T1 (C++).pdf' → 'C1T1' — ตัดวงเล็บท้ายชื่อแบบเดียวกับ organize.fish"""
    stem = os.path.splitext(filename)[0]
    return re.sub(r"\s*\([^)]*\)", "", stem).strip()


def _resolve(path: str) -> str:
    """path สัมพัทธ์คิดจากราก repo (เหมือน organize.fish)"""
    return path if os.path.isabs(path) else os.path.join(REPO_ROOT, path)


def plan_for_dir(src_dir: str, code: str) -> tuple[dict[str, str | None], list[str]]:
    """วางแผนว่าไฟล์ไหนไปไหน → ({ชื่อปลายทาง: ชื่อต้นทาง}, ไฟล์ที่ข้าม/ไม่รู้จัก)

    ชื่อปลายทาง None = ต้องสร้างจาก template.cpp (ข้อที่ต้นทางไม่มีโค้ด)
    """
    moves: dict[str, str | None] = {}
    skipped: list[str] = []
    pdfs: list[str] = []

    for name in sorted(os.listdir(src_dir)):
        full = os.path.join(src_dir, name)
        if os.path.isdir(full):
            skipped.append(name)
            continue

        lower = name.lower()
        if lower.endswith(".pdf"):
            pdfs.append(name)
        elif name == "main.cpp":
            moves[f"{code}.cpp"] = name
        elif name == "main_optimize.cpp":
            moves[f"{code}_optimize.cpp"] = name
        elif lower in _SKIP_NAMES or os.path.splitext(lower)[1] in _SKIP_EXTS:
            skipped.append(name)
        else:
            skipped.append(name)

    # ปกติมี PDF เดียวชื่อ "<รหัส> (th).pdf" — ถ้าหลายไฟล์เลือกอันที่ชื่อตรงรหัส
    # ไม่มี PDF เลยก็ไม่ใส่คีย์ (ผู้เรียกจะได้เตือนว่าต้องโหลดเอง)
    if pdfs:
        exact = [p for p in pdfs if _pdf_stem(p).upper() == code.upper()]
        chosen = exact[0] if exact else pdfs[0]
        moves[f"{code}.pdf"] = chosen
        skipped.extend(p for p in pdfs if p != chosen)

    if f"{code}.cpp" not in moves:
        moves[f"{code}.cpp"] = None  # ต้นทางไม่มีโค้ด — สร้างจาก template

    return moves, skipped


def main() -> int:
    parser = argparse.ArgumentParser(
        description="ยกโจทย์+โค้ดจากโฟลเดอร์ค่ายปีเก่า เข้ามาเป็นโฟลเดอร์โจทย์ของ repo นี้",
    )
    parser.add_argument("source", help="โฟลเดอร์ต้นทาง เช่น ~/Academics/OCom_2025/Camp1/.../1.CMU_Grader")
    parser.add_argument("--contest", required=True, help="ชื่อ contest = ชื่อไฟล์ใน tools/scores/ เช่น camp1_1")
    parser.add_argument("--dest", required=True, help="โฟลเดอร์ปลายทางของ grader เช่น problems/2_incamp/1_first_half/cmu-grader")
    parser.add_argument("--dry-run", action="store_true", help="ดูเฉย ๆ ยังไม่คัดลอก")
    parser.add_argument("--move", action="store_true", help="ย้ายจริงแทนการคัดลอก")
    args = parser.parse_args()

    grader = get_grader(args.contest)
    if grader is None or not grader.has_score:
        print(f"❌ ไม่เจอไฟล์คะแนนของ contest «{args.contest}» ใน tools/scores/")
        print("   วางตารางคะแนนก่อน แล้วค่อยรัน (ดูรายชื่อ contest ที่ tools/graders.txt)")
        return 1

    src_root = os.path.realpath(os.path.expanduser(args.source))
    dest_root = _resolve(args.dest)
    template = os.path.join(REPO_ROOT, "template.cpp")

    if not os.path.isdir(src_root):
        print(f"❌ ไม่เจอโฟลเดอร์ต้นทาง {src_root}")
        return 1
    if not args.dry_run:
        os.makedirs(dest_root, exist_ok=True)

    # ลำดับ + ชื่อโฟลเดอร์มาจากไฟล์คะแนน (ลำดับบรรทัด = ลำดับเดียวกับตารางใน README)
    tasks = parse_score(grader.score_path)
    targets = {t.task_id: folder_name_for(t.task_id, t.name, order) for order, t in enumerate(tasks, 1)}

    found: dict[str, str] = {}
    unmatched: list[str] = []
    for name in sorted(os.listdir(src_root)):
        full = os.path.join(src_root, name)
        if not os.path.isdir(full):
            continue
        m = _SRC_DIR_RE.match(name)
        if not m:
            unmatched.append(name)
            continue
        code = m.group(1)
        found[code] = full

    action = "ย้าย" if args.move else "คัดลอก"
    print(f"📥 {action}จาก {src_root}")
    print(f"   → {os.path.relpath(dest_root, REPO_ROOT)}  ({len(targets)} ข้อในไฟล์คะแนน)")
    print("-" * 60)

    copied = created = 0
    no_pdf: list[str] = []
    for task_id, folder in targets.items():
        src_dir = found.get(task_id)
        dest_dir = os.path.join(dest_root, folder)

        if src_dir is None:
            continue  # ไม่มีต้นทาง (โจทย์ใหม่) — ปล่อยให้ organize.fish จัดจาก PDF ที่โหลดมา

        moves, skipped = plan_for_dir(src_dir, task_id)
        if f"{task_id}.pdf" not in moves:
            no_pdf.append(task_id)

        if os.path.isdir(dest_dir) and os.listdir(dest_dir):
            print(f"⚠️  {task_id}: มี {folder} อยู่แล้ว — ข้าม")
            continue

        if args.dry_run:
            files = ", ".join(
                f"{dst}←{src}" if src else f"{dst}←template.cpp" for dst, src in moves.items()
            )
            print(f"👀 {folder}\n     {files}")
            if skipped:
                print(f"     ข้าม: {' '.join(skipped)}")
            copied += 1
            continue

        os.makedirs(dest_dir, exist_ok=True)
        for dst_name, src_name in moves.items():
            dst = os.path.join(dest_dir, dst_name)
            if src_name is None:
                if os.path.isfile(template):
                    shutil.copy2(template, dst)
                    created += 1
                else:
                    open(dst, "w", encoding="utf-8").close()
                    created += 1
            else:
                src = os.path.join(src_dir, src_name)
                if args.move:
                    shutil.move(src, dst)
                else:
                    shutil.copy2(src, dst)
                copied += 1
        print(f"✅ {folder}")

    print("-" * 60)

    # ข้อในไฟล์คะแนนที่ต้นทางไม่มี — ปกติคือโจทย์ใหม่ของปีนี้
    missing_src = [tid for tid in targets if tid not in found]
    if missing_src:
        print(f"🆕 ไม่มีต้นทาง {len(missing_src)} ข้อ (ปกติ = โจทย์ใหม่): {' '.join(missing_src)}")

    if no_pdf:
        print(f"⚠️  ต้นทางไม่มี PDF {len(no_pdf)} ข้อ (โหลดเองทีหลัง): {' '.join(no_pdf)}")

    unknown = sorted(set(found) - set(targets))
    if unknown:
        print(f"⚠️  ต้นทางมีแต่ไม่รู้จัก {len(unknown)} ข้อ (ข้าม): {' '.join(unknown)}")

    if unmatched:
        print(f"⚠️  โฟลเดอร์ต้นทางที่ชื่อไม่เข้าแบบ <เลข>.<รหัส> (ข้าม): {' '.join(unmatched)}")

    if args.dry_run:
        print(f"👀 จะ{action} {copied} ข้อ — โหมด --dry-run ยังไม่แตะอะไร")
    else:
        print(f"🎉 {action}เสร็จ · คัดลอกไฟล์ {copied} · สร้างจาก template {created} ไฟล์")
        print("   ต่อไป: python3 tools/find_missed.py   แล้ว   python3 tools/sync_done.py")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
