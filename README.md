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
│   ├── 2_incamp/          ระหว่างค่าย — แยกตามช่วงเหมือนที่เว็บแบ่ง
│   │   ├── 1_first_half/cmu-grader/    + _done/
│   │   ├── 2_second_half/cmu-grader/   + _done/
│   │   ├── exam/ex1/                   + _done/
│   │   ├── exam/ex2/                   + _done/
│   │   ├── pretest/                    + _done/
│   │   ├── yrc-grader/                 + _done/
│   │   └── sheets/          โค้ดตัวอย่างที่อาจารย์แจก
│   └── 3_endcamp/         หลังค่าย
│
├── notes/               ← สรุป / cheatsheet ที่เขียนเอง
└── tools/               ← สคริปต์ช่วยงาน — ดู tools/README.md
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

โจทย์โหลดมือจาก <https://www.cs.science.cmu.ac.th/compo/> — ไม่มีสคริปต์ดึงอัตโนมัติ

```fish
# 1. เปิดหน้าเว็บของ contest โหลด Statement ที่ยังขาดมาก่อน
#    (ดูว่าข้อไหนขาดด้วยคำสั่งนี้ — ไม่แก้อะไร แค่อ่าน)
python3 tools/find_missed.py

# 2. จัด PDF ที่โหลดมาเข้าโฟลเดอร์ + สร้าง .cpp จาก template ให้เลย
fish tools/organize.fish ~/Downloads --cpp

#    สไลด์/ชีทเรียนก็ใช้ตัวเดียวกัน แต่มักต้องเติมวันที่นำหน้า
fish tools/organize.fish docs/sheets --prefix 20261005_AM --dry-run

# 3. เขียนโค้ดที่  problems/1_precamp/cmu-grader/C1PR01/C1PR01.cpp
#    กด Ctrl+Shift+B ใน VSCode เพื่อ build + run (อ่าน input จาก .run/input.in)

# 4. ก็อปตารางคะแนนจากหน้าเว็บ → tools/scores/precamp.txt แล้วรัน
python3 tools/sync_done.py     # ย้ายข้อที่ได้ 100 → _done/ และอัปเดตตารางข้างล่าง
```

รหัสโจทย์ไม่ซ้ำกันระหว่างค่าย — `sync_done.py` หาโฟลเดอร์เจอเองโดยไม่ต้องบอกว่า
ข้อไหนอยู่ช่วงไหน ส่วนรายชื่อ contest ทั้งหมดดูที่ `tools/graders.txt`

---

## 🏆 ตารางคะแนน

> สร้างอัตโนมัติโดย `python3 tools/sync_done.py` จาก `tools/scores/*.txt` — อย่าแก้มือ

<!-- SCORE:START -->
_ยังไม่มีข้อมูล — วางตารางคะแนนจากหน้าเว็บลง `tools/scores/<ชื่อ contest>.txt` แล้วรัน `python3 tools/sync_done.py`_
<!-- SCORE:END -->

---

## 📝 สรุปที่เขียนเอง

| หัวข้อ | ไฟล์ |
|---|---|
| _(ยังไม่มี)_ | |
