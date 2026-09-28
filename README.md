# 💻 สอวน. คอมพิวเตอร์ ค่าย 1 — 2569

เวิร์กสเปซส่วนตัวสำหรับค่าย 1 สอวน. คอมพิวเตอร์ ปีการศึกษา 2569
รวมโจทย์ สไลด์ โค้ดที่เขียน และสรุปที่อ่านเองไว้ที่เดียว

> ของปีก่อน: [SecondHalf_Of_OlympicComputer_2568](../../../OCom_2025/Camp1/SecondHalf_Of_OlympicComputer_2568)
> · [FirstHalf_Of_OlympicComputer_2568](../../../OCom_2025/Camp1/FirstHalf_Of_OlympicComputer_2568)
> · [Pre_camp1_Olympic](../../../OCom_2025/Camp1/Pre_camp1_Olympic)
>
> ดูโครงของค่าย 2 ได้ที่ [olympicComputer-camp2](../../../OCom_2025/Camp2/olympicComputer-camp2)

---

## 📁 โครงสร้าง

```
olympicComputer-camp1-2569/
├── README.md            ← ไฟล์นี้ (สารบัญ + ตารางคะแนน)
├── CLAUDE.md            ← บริบทให้ AI ช่วยงาน
├── template.cpp         ← โค้ดตั้งต้นสำหรับโจทย์ใหม่
├── .vscode/tasks.json   ← ปุ่ม build + run
│
├── docs/                ← เอกสารค่าย (ที่ไม่ใช่โจทย์)
│   ├── syllabus/          กำหนดการ, syllabus, เอกสารสมัคร
│   ├── sheets/            สไลด์/ชีทเรียน  →  YYYYMMDD_AM_Topic/
│   └── exams/             ตัวข้อสอบ pretest / midterm / final
│
├── problems/            ← โจทย์ทั้งหมด
│   ├── 1_precamp/         พรีแคมป์ (ก่อนเข้าค่าย)
│   │   ├── cmu-grader/      + _done/  ← C1PC* C1PE* C1PR*
│   │   ├── codeforces/
│   │   └── programming.in.th/
│   ├── 2_incamp/          ระหว่างค่าย
│   │   ├── cmu-grader/      + _done/  ← ย้ายอัตโนมัติเมื่อได้ 100
│   │   ├── yrc-grader/      + _done/
│   │   ├── pretest/
│   │   ├── exam/
│   │   └── sheets/          โค้ดตัวอย่างที่อาจารย์แจก
│   └── 3_endcamp/         หลังค่าย
│
├── notes/               ← สรุป / cheatsheet ที่เขียนเอง
└── tools/               ← สคริปต์ช่วยงาน
```

### กติกาการตั้งชื่อ

| อะไร | ตั้งชื่อว่า | ตัวอย่าง |
|---|---|---|
| โฟลเดอร์โจทย์ 1 ข้อ | รหัสโจทย์ตรงกับบน grader | `C1PR01/` |
| ไฟล์ในโฟลเดอร์โจทย์ | ชื่อเดียวกับโฟลเดอร์ | `C1PR01.cpp`, `C1PR01.pdf` |
| statement ที่เว็บให้มาเป็น HTML | เก็บเป็น `.html` | `C1PR01.html` |
| สไลด์/ชีทเรียน | `YYYYMMDD_AM\|PM_หัวข้อ` | `20261005_AM_BasicC/` |
| เทสเคส | `testcases/1.in`, `1.out` | ซ้อนในโฟลเดอร์โจทย์ |

- **ไม่ใส่เลขนำหน้า** โฟลเดอร์โจทย์ — เรียงตามรหัสอยู่แล้ว (`C1P01` < `C1P02` < `C1T01`)
- เลขนำหน้ามีแค่ระดับ phase (`1_precamp` → `2_incamp` → `3_endcamp`) เพราะต้องเรียงตามเวลา
- ไฟล์ที่คอมไพล์ออกมา (`.out`, `.exe`) ไม่ขึ้น git — ดู `.gitignore`

---

## 🔄 ขั้นตอนทำงาน

```fish
# 0. ตั้งค่าล็อกอินครั้งเดียว — คัดลอกแล้วเติมรหัส
cp tools/secrets.fish.example tools/secrets.fish

# 1. ดึง statement + สร้างโฟลเดอร์ + ไฟล์ .cpp จาก template ให้เลย
fish tools/fetch_statements.fish C1PR01 C1PR02 C1PR03

#    หรือดึงทั้งชุดที่ระบุไว้ใน tools/tasks.txt (ตอนนี้มี 36 ข้อของพรีแคมป์)
fish tools/fetch_statements.fish --from-list

#    ล็อกอินค้างอยู่ที่ tools/.cookies.txt — รันครั้งต่อไปไม่ต้องล็อกอินใหม่
#    ถ้า session หมดอายุกลางทาง สคริปต์จะล็อกอินให้ใหม่เองหนึ่งครั้ง

# 1b. ไฟล์ที่โหลดมือมาแบบลอย ๆ (สไลด์/โจทย์นอก grader) — จัดเข้าโฟลเดอร์ให้
fish tools/organize.fish docs/sheets --prefix 20261005_AM --dry-run

# 2. เขียนโค้ดที่  problems/1_precamp/cmu-grader/C1PR01/C1PR01.cpp
#    กด Ctrl+Shift+B ใน VSCode เพื่อ build + run (อ่าน input จาก .run/input.in)

# 3. ก็อปตารางคะแนนจากหน้า grader มาวางทับ tools/score.txt แล้วรัน
python3 tools/sync_done.py     # ย้ายข้อที่ได้ 100 → _done/ และอัปเดตตารางข้างล่าง
python3 tools/find_missed.py   # เช็คว่าข้อไหนใน score.txt ยังไม่มีโฟลเดอร์
```

`tools/secrets.fish` กับ `tools/.cookies.txt` เก็บรหัสและ session ของ grader
— **ไม่ขึ้น git** (ดู `tools/secrets.fish.example`)

---

## 🏆 ตารางคะแนน

> สร้างอัตโนมัติโดย `python3 tools/sync_done.py` จาก `tools/score.txt` — อย่าแก้มือ

<!-- SCORE:START -->
_ยังไม่มีข้อมูล — วางตารางคะแนนจาก grader ลง `tools/score.txt` แล้วรัน `python3 tools/sync_done.py`_
<!-- SCORE:END -->

---

## 📝 สรุปที่เขียนเอง

| หัวข้อ | ไฟล์ |
|---|---|
| _(ยังไม่มี)_ | |
