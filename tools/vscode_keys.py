#!/usr/bin/env python3
"""ติดตั้งคีย์ลัด Ctrl+F5 = build + run ให้ VS Code

VS Code **ไม่มี** keybindings ระดับ workspace (ไม่มี `.vscode/keybindings.json`) —
คีย์ลัดเป็นเรื่องของ "เครื่องที่เปิดหน้าจอ" เท่านั้น ต้องเขียนลงไฟล์ของผู้ใช้:
  Linux    ~/.config/Code/User/keybindings.json
  macOS    ~/Library/Application Support/Code/User/keybindings.json
  Windows  %APPDATA%\\Code\\User\\keybindings.json
สคริปต์นี้หาโฟลเดอร์นั้นเอง (รวม VSCodium / Cursor / Insiders / flatpak)

วิธีใช้
    python3 tools/vscode_keys.py            # ดูเฉย ๆ ว่าจะเขียนอะไร ลงที่ไหน
    python3 tools/vscode_keys.py --apply    # เขียนจริง  (รันซ้ำได้ ไม่ซ้อนกัน)
    python3 tools/vscode_keys.py --print    # พิมพ์ JSON ให้ก็อปวางเอง (ไม่แตะไฟล์)
    python3 tools/vscode_keys.py --apply --remove        # ถอนคีย์ลัดออก
    python3 tools/vscode_keys.py --apply --dir ~/.config/Code/User

คีย์ลัดที่ติดตั้ง
    Ctrl+F5 → task "C++: build + run" (เปิดไฟล์ .cpp อยู่)
    แก้คีย์ได้ด้วย --key · จำกัดเฉพาะ repo นี้ด้วย --only-this-repo

⚠️ เครื่องที่รัน VS Code กับเครื่องที่รันคำสั่งนี้ต้องเป็นเครื่องเดียวกัน —
   เครื่องนี้ (VPS) ไม่มี VS Code ถ้าไม่เจอโฟลเดอร์ VS Code สคริปต์จะบอกให้ไปรัน
   บนเครื่องที่เปิดหน้าจอ (หรือรันด้วย --print แล้วก็อปไปวางเอง)

⚠️ `args` อ้าง **label ของ task** — ถ้าแก้ label ใน `.vscode/tasks.json`
   คีย์ลัดจะเงียบ (ขึ้นให้เลือก task) สคริปต์นี้เช็ค label ให้แล้ว
   ถ้าไม่ตรงจะเตือนทุกโหมด และ **หยุดก่อน** ในโหมด --apply (ไม่มีอะไรถูกเขียน)
"""

from __future__ import annotations

import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from scorelib import REPO_ROOT  # noqa: E402

TASK_LABEL = "C++: build + run"
DEFAULT_KEY = "ctrl+f5"

MARK_OPEN = "// >>> olympicComputer-camp1-2569 >>>"
MARK_CLOSE = "// <<< olympicComputer-camp1-2569 <<<"

# ชื่อโฟลเดอร์ของแต่ละตัว (VS Code · Insiders · VSCodium · Cursor)
EDITOR_DIRNAMES = ("Code", "Code - Insiders", "VSCodium", "Cursor")

# คอมเมนต์ JSONC หรือ string — ใช้ "กลบ" ให้ความยาวเท่าเดิม เพื่อวิเคราะห์โครงสร้าง
# โดยไม่ถูกวงเล็บ/คอมมา/ปีกกา ที่ซ่อนอยู่ในคอมเมนต์หรือข้อความหลอก
_TOKEN_RE = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"", re.S)


def masked(text: str) -> str:
    """แทนคอมเมนต์ด้วยช่องว่าง และกลบใน string — ตำแหน่งตัวอักษรยังตรงเดิม"""
    def rep(m: re.Match[str]) -> str:
        s = m.group(0)
        if s.startswith(("//", "/*")):
            return " " * len(s)
        return '"' + " " * (len(s) - 2) + '"'      # เก็บปีกกาไว้ ตำแหน่งจะได้ตรง
    return _TOKEN_RE.sub(rep, text)


def newline_of(text: str) -> str:
    """บรรทัดจบของไฟล์นั้น — VS Code บน Windows เขียน CRLF ถ้าไม่บอกไว้
    แทรกด้วย LF ทับลงไปจะได้ไฟล์ที่จบสองแบบปนกัน (อ่านได้ แต่ diff รก)"""
    crlf = text.count("\r\n")
    lf = text.count("\n") - crlf
    return "\r\n" if crlf > lf else "\n"


# ---------------------------------------------------------------- หาโฟลเดอร์ผู้ใช้
def user_dirs() -> list[str]:
    """โฟลเดอร์ User ของ editor ทุกตัวที่เจอบนเครื่องนี้"""
    home = os.path.expanduser("~")
    found: list[str] = []

    def add(p: str) -> None:
        if p and os.path.isdir(p) and p not in found:
            found.append(p)

    if sys.platform.startswith("win"):
        base = os.environ.get("APPDATA", "")
        for n in EDITOR_DIRNAMES:
            add(os.path.join(base, n, "User"))
    elif sys.platform == "darwin":
        for n in EDITOR_DIRNAMES:
            add(os.path.join(home, "Library", "Application Support", n, "User"))
    else:
        for n in EDITOR_DIRNAMES:
            add(os.path.join(home, ".config", n, "User"))
        # flatpak — VS Code เก็บ config ใน ~/.var ไม่ใช่ ~/.config
        add(os.path.join(home, ".var", "app", "com.visualstudio.code",
                         "config", "Code", "User"))
    # เผื่อตั้ง XDG_CONFIG_HOME ไว้
    xdg = os.environ.get("XDG_CONFIG_HOME")
    if xdg and not sys.platform.startswith("win"):
        for n in EDITOR_DIRNAMES:
            add(os.path.join(xdg, n, "User"))
    return found


# ---------------------------------------------------------------- ตัวบล็อกที่แทรก
def block_for(key: str, task: str, when: str, leading_comma: bool = False,
              nl: str = "\n") -> str:
    """ข้อความที่แทรกลง array — คอมมาคั่น (ถ้าต้องมี) อยู่ "ข้างใน" ช่วง marker

    วางไว้ใน span เพราะตอนถอนจะได้ไม่ต้องเดาว่าคอมมาตัวไหนเป็นของเรา — ถ้าเอาไป
    ต่อท้ายคีย์ลัดตัวก่อนหน้า จะแยกไม่ออกระหว่างคอมมาที่เราเติมกับคอมมาท้าย array
    ที่ผู้ใช้มีอยู่แล้ว แล้วถอนทีไรก็กินคอมมาของผู้ใช้ทิ้ง

    `nl` = บรรทัดจบของไฟล์ปลายทาง (CRLF บน Windows) — ผู้เรียกที่พิมพ์ออกจอใช้ค่าเริ่มต้น
    """
    out = [f"  {MARK_OPEN} — อย่าลบสองบรรทัด marker นี้ (tools/vscode_keys.py ใช้หา)"]
    if leading_comma:
        out.append("  ,  // ← ตัวคั่นจากคีย์ลัดตัวก่อนหน้า")
    out += [
        f"  // {key} = build + run (งานเดียวกับ Ctrl+Shift+B แต่รันต่อให้เลย)",
        "  {",
        f'    "key": "{key}",',
        '    "command": "workbench.action.tasks.runTask",',
        f'    "args": "{task}",',
        f'    "when": "{when}"',
        "  }",
        f"  {MARK_CLOSE}",
    ]
    return nl.join(out)


def needs_leading_comma(prefix_masked: str) -> bool:
    """ต้องเติมคอมมาหน้าบล็อกไหม — ดูจากสิ่งที่อยู่ก่อนหน้าว่าเป็น array ว่างหรือลงท้ายด้วยคอมมา"""
    s = prefix_masked.rstrip()
    return bool(s) and s != "[" and not s.endswith(",")


def when_clause(only_this_repo: bool) -> str:
    base = "editorLangId == 'cpp'"
    if not only_this_repo:
        return base
    repo = os.path.basename(REPO_ROOT)
    # resourceDirname เป็น context ของ keybindings ได้จริง — แต่จะเป็นเท็จเมื่อไม่มี
    # ไฟล์เปิดอยู่ กดแล้วเงียบ · จึงใช้ตัวเลือก ไม่ใช่ค่าเริ่มต้น
    return f"{base} && resourceDirname =~ /{repo}/"


def mark_span(text: str) -> tuple[int, int] | None:
    """ช่วงของบล็อกที่เราเคยเขียน (เริ่มที่ marker เปิด จบที่ marker ปิด) — ไม่รวมคอมมา"""
    i = text.find(MARK_OPEN)
    j = text.find(MARK_CLOSE)
    if i < 0 or j < 0 or j < i:
        return None
    return (i, j + len(MARK_CLOSE))


def insert_into(text: str, key: str, task: str, when: str) -> str:
    """แทรกคีย์ลัดเป็นสมาชิกตัวสุดท้ายของ array — คงคอมเมนต์/คีย์ลัดเดิมไว้ทั้งหมด"""
    if text.strip() == "":
        return f"[\n{block_for(key, task, when)}\n]\n"

    nl = newline_of(text)                       # ไฟล์ CRLF (Windows) ต้องได้ CRLF ทั้งบล็อก
    m = masked(text)
    span = mark_span(text)
    if span:                                    # เคยเขียนไว้ → ทับของเดิม
        i, j = span
        rest = re.sub(r"^\s*,", "", text[j:])   # ดูดคอมมาของรอบก่อน (ถ้ามี) ทิ้ง
        lead = needs_leading_comma(m[:i])
        # mark_span เริ่มที่ตัว `//` ไม่ใช่ที่ช่องว่างข้างหน้า ⇒ `text[:i]` ยังมี
        # ย่อหน้าของบรรทัด marker ติดมาด้วย ถ้าไม่ตัดออก ทับรอบที่สองจะได้ `    // >>>`
        # แล้วยาวขึ้นทีละ 2 ช่องทุกครั้งที่รัน --apply (รันซ้ำได้ไม่จริง)
        head = re.sub(r"[ \t]+$", "", text[:i])
        return head + block_for(key, task, when, lead, nl) + rest

    open_pos, close_pos = m.find("["), m.rfind("]")
    if open_pos < 0 or close_pos < open_pos:
        raise ValueError(
            "อ่าน keybindings.json ไม่ได้ — ไม่เจอ array ปีกกาเหลี่ยม\n"
            "   (ไฟล์นี้น่าจะเสีย ถ้าเพิ่งแก้มือให้เปิด VS Code แล้วดูว่าเตือน error ไหม)"
        )
    lead = needs_leading_comma(m[open_pos + 1:close_pos])
    head, tail = text[:close_pos].rstrip(), text[close_pos:]
    return head + nl + block_for(key, task, when, lead, nl) + nl + tail


def remove_from(text: str) -> str | None:
    """ถอนบล็อกของเราออก — คืน None ถ้าไม่เคยมี · ไฟล์กลับไปเหมือนเดิมเป๊ะ"""
    span = mark_span(text)
    if not span:
        return None
    i, j = span
    # บล็อกถูกแทรกเป็นตัวสุดท้ายเสมอ ⇒ ขึ้นบรรทัดใหม่หน้า marker กับคอมมาที่เราเติม
    # ต้องเก็บออกทั้งคู่ ไม่งั้นถอนแล้วจะเหลือบรรทัดว่างโผล่มาก่อน `]`
    # ⚠️ lstrip ต้องกิน \r ด้วย ไม่ใช่แค่ \n ไม่งั้นไฟล์ CRLF จะเหลือบรรทัดว่างเกินมา
    nl = newline_of(text)
    out = text[:i].rstrip() + nl + re.sub(r"^\s*,", "", text[j:]).lstrip("\r\n")
    if not masked(out).partition("[")[2].rpartition("]")[0].strip():
        out = "[]" + nl                 # เหลือแต่ปีกกาเปล่า — ให้เป็น `[]` สวย ๆ
    return out


# ---------------------------------------------------------------- ตรวจ task label
def check_task_label(label: str) -> str | None:
    """เช็คว่า label มีจริงใน .vscode/tasks.json — คืนคำเตือนถ้าไม่มี"""
    path = os.path.join(REPO_ROOT, ".vscode", "tasks.json")
    if not os.path.isfile(path):
        return f"ไม่เจอ {os.path.relpath(path, REPO_ROOT)} — เช็คเองว่า task ชื่อ '{label}' มีอยู่"
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            raw = f.read()
    except OSError:
        return None
    # tasks.json เป็น JSONC มีคอมเมนต์ — glob label ด้วย regex ตรง ๆ ง่ายและพอ
    labels = re.findall(r'"label"\s*:\s*"((?:[^"\\]|\\.)*)"', raw)
    if label in labels:
        return None
    listed = " · ".join(f"'{x}'" for x in labels) or "ไม่มีเลย"
    return (f"⚠️  ใน .vscode/tasks.json ไม่มี task ชื่อ '{label}'\n"
            f"   ที่มีอยู่: {listed}\n"
            f"   → แก้ label ให้ตรง หรือคีย์ลัดจะขึ้นให้เลือก task เองแทน")


# ---------------------------------------------------------------- main
def main() -> int:
    ap = argparse.ArgumentParser(
        prog="vscode_keys.py",
        description="ติดตั้ง Ctrl+F5 = build + run ให้ VS Code (เขียนที่ keybindings.json ของผู้ใช้)",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("--apply", action="store_true", help="เขียนจริง (ไม่ใส่ = ดูเฉย ๆ)")
    ap.add_argument("--print", dest="print_only", action="store_true",
                    help="พิมพ์ JSON ให้ก็อปวางเอง ไม่แตะไฟล์")
    ap.add_argument("--remove", action="store_true", help="ถอนคีย์ลัดที่เราเคยติดตั้ง")
    ap.add_argument("--key", default=DEFAULT_KEY, help=f"คีย์ลัด (default: {DEFAULT_KEY})")
    ap.add_argument("--task", default=TASK_LABEL, help=f"label ของ task (default: {TASK_LABEL})")
    ap.add_argument("--only-this-repo", action="store_true",
                    help="ให้คีย์ลัดทำงานเฉพาะโปรเจกต์นี้ (ค่าเริ่มต้น: ทุกไฟล์ .cpp)")
    ap.add_argument("--dir", default="", metavar="PATH", help="บังคับโฟลเดอร์ User เอง")
    args = ap.parse_args()

    when = when_clause(args.only_this_repo)
    # โชว์แบบมีคอมมาคั่น (กรณีปกติคือวางต่อท้ายของเดิม) แล้วบอกวิธีปรับ
    block = block_for(args.key, args.task, when, leading_comma=True)

    # เตือนตรงนี้เลย ไม่ใช่ตอนจะเขียนไฟล์ — เพราะเครื่องที่ไม่มี VS Code (เช่น VPS นี้)
    # จะไปจบที่ `--print` หรือทางออก "ไม่เจอโฟลเดอร์" ซึ่งทั้งคู่ return ก่อนถึงจุดนั้น
    # ⇒ ถ้าเตือนหลังตรงนี้ ผู้ใช้กลุ่มนั้น (ซึ่งก็อป JSON ไปวางเอง) จะไม่เห็นคำเตือนเลย
    warn = check_task_label(args.task)
    if warn:
        print(warn)
        if args.apply and not args.print_only:
            print("   → หยุดก่อน ยังไม่ได้เขียนอะไร (แก้ label ให้ตรงแล้วรันใหม่)")
            return 1

    if args.print_only:
        print("วางใน keybindings.json (ใน array ปีกกาเหลี่ยม):")
        print()
        print(block)
        print()
        print("⚠️ ถ้า array ยังว่างเปล่า หรือคีย์ลัดตัวสุดท้ายมีคอมมาอยู่แล้ว")
        print("   ให้ลบบรรทัด `  ,  // ← ตัวคั่น...` ออก (ไม่ลบจะได้คอมมาซ้อน = JSON พัง)")
        print()
        print("ใน VS Code: Ctrl+Shift+P → 'Preferences: Open Keyboard Shortcuts (JSON)'")
        return 0

    dirs = [os.path.abspath(os.path.expanduser(args.dir))] if args.dir else user_dirs()
    if not dirs:
        print("❌ ไม่เจอโฟลเดอร์ของ VS Code บนเครื่องนี้")
        print()
        print(f"   เครื่องนี้ ({sys.platform}) ค้นหาใน:")
        home = os.path.expanduser("~")
        for n in EDITOR_DIRNAMES:
            print(f"     · {os.path.join(home, '.config', n, 'User')}")
        print()
        print("   ถ้าเปิด VS Code อยู่บนเครื่องอื่น (เช่นเครื่องที่คุณนั่งทำ) ให้รันสคริปต์นี้")
        print("   บนเครื่องนั้นแทน — หรือก็อป JSON นี้ไปวางเองด้วย `--print`:")
        print()
        print(block)
        print()
        print("   เปิดไฟล์ด้วย Ctrl+Shift+P → 'Preferences: Open Keyboard Shortcuts (JSON)'")
        print("   แล้ววางใน array ปีกกาเหลี่ยม")
        return 2

    rc = 0
    for d in dirs:
        path = os.path.join(d, "keybindings.json")
        action = "ถอน" if args.remove else "ติดตั้ง"
        exists = os.path.isfile(path)
        print(f"🎹 {action}: {path}" + ("" if exists else "  (ยังไม่มีไฟล์ — จะสร้างให้)"))

        if not args.apply:
            if args.remove:
                if exists:
                    got = remove_from(open(path, encoding="utf-8").read())
                    print("   · " + ("จะลบบล็อกของเราออก" if got is not None
                                    else "ไม่พบบล็อกของเรา — ไม่มีอะไรต้องลบ"))
                else:
                    print("   · ยังไม่มีไฟล์ — ไม่มีอะไรต้องลบ")
            else:
                print(f"   · จะแทรกคีย์ลัด {args.key} → task '{args.task}'")
                print(f"   · when: {when}")
                print("   · คอมเมนต์และคีย์ลัดเดิมของคุณไม่ถูกแตะ")
            continue

        try:
            text = open(path, encoding="utf-8").read() if exists else ""
        except OSError as e:
            print(f"   ❌ อ่านไม่ได้: {e}")
            rc = 1
            continue

        try:
            if args.remove:
                new = remove_from(text)
                if new is None:
                    print("   · ไม่พบบล็อกของเรา — ไม่มีอะไรต้องลบ")
                    continue
            else:
                new = insert_into(text, args.key, args.task, when)
        except ValueError as e:
            print(f"   ❌ {e}")
            rc = 1
            continue

        try:
            os.makedirs(d, exist_ok=True)
            with open(path, "w", encoding="utf-8") as f:
                f.write(new)
        except OSError as e:
            print(f"   ❌ เขียนไม่ได้: {e}")
            rc = 1
            continue
        print("   ✅ เรียบร้อย — VS Code อ่านไฟล์นี้ใหม่ทันที ไม่ต้องปิดเปิด")
        if not args.remove:
            print(f"      ลองเปิดไฟล์ .cpp แล้วกด {args.key}")

    if not args.apply:
        print()
        print("ℹ️  นี่แค่ตัวอย่าง — ยังไม่ได้เขียนอะไร ใส่ --apply เพื่อเขียนจริง")
    return rc


if __name__ == "__main__":
    raise SystemExit(main())
