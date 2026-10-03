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
│   │   ├── cmu-grader/      + _done/  ← 01_C1PC01_Alice_and_Bob/ ฯลฯ
│   │   ├── codeforces/
│   │   └── programming.in.th/
│   ├── 2_incamp/          ระหว่างค่าย — แยกตามช่วงเหมือนที่เว็บแบ่ง
│   │   ├── 1_first_half/    cmu-grader/ · yrc-grader/   (+ _done/ ในแต่ละอัน)
│   │   ├── 2_second_half/   cmu-grader/ · yrc-grader/   (+ _done/)
│   │   ├── exam/ex1/                    + _done/
│   │   ├── exam/ex2/                    + _done/
│   │   ├── pretest/                     + _done/
│   │   └── sheets/          โค้ดตัวอย่างที่อาจารย์แจก
│   └── 3_endcamp/         หลังค่าย
│
├── notes/               ← สรุป / cheatsheet ที่เขียนเอง
└── tools/               ← สคริปต์ช่วยงาน — ดู tools/README.md
```

### กติกาการตั้งชื่อ

| อะไร | ตั้งชื่อว่า | ตัวอย่าง |
|---|---|---|
| โฟลเดอร์โจทย์ 1 ข้อ | `<ลำดับ>_<รหัสโจทย์>_<ชื่อโจทย์>` | `01_C1PC01_Alice_and_Bob/` |
| ไฟล์ในโฟลเดอร์โจทย์ | **รหัสโจทย์เปล่า ๆ** (ไม่ต่อชื่อ) | `C1PC01.cpp`, `C1PC01.pdf` |
| statement ที่เว็บให้มาเป็น HTML | เก็บเป็น `.html` | `C1PC01.html` |
| สไลด์/ชีทเรียน | `YYYYMMDD_AM\|PM_หัวข้อ` | `20261005_AM_BasicC/` |
| เทสเคส | `testcases/1.in`, `1.out` | ซ้อนในโฟลเดอร์โจทย์ |

- **โฟลเดอร์มีเลขนำหน้า** เพราะลำดับโจทย์บนเว็บไม่เหมือนลำดับรหัส (เว็บเรียง C1PC → C1PR →
  C1PE แต่เรียงตามตัวอักษรได้ C1PC → C1PE → C1PR) เลขลำดับมาจากบรรทัดใน
  `tools/scores/*.txt` — ตัวเดียวกับตารางคะแนนข้างล่าง
- **ไฟล์ข้างในยังเป็นรหัสเปล่า ๆ** เพราะ grader บังคับชื่อไฟล์ตอน submit (คอลัมน์
  Files = `C1PC01[.cpp|.c]`) ถ้าใส่ชื่อโจทย์ไว้ต้อง rename กลับก่อนส่งทุกครั้ง
  โฟลเดอร์กับไฟล์จึงชื่อไม่เหมือนกันโดยตั้งใจ
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

# 3. เขียนโค้ดที่  problems/1_precamp/cmu-grader/13_C1PR01_Test/C1PR01.cpp
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
### พรีแคมป์ — ได้เต็ม **0 / 36** ข้อ

| สถานะ | คะแนน | Task | ชื่อโจทย์ | ลิมิต | ไฟล์ |
| :---: | :---: | :--- | :--- | :---: | :---: |
| 🔴 | 0 / 100 | **C1PC01** | Alice and Bob | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/01_C1PC01_Alice_and_Bob) |
| 🔴 | 0 / 100 | **C1PC02** | Existence | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/02_C1PC02_Existence) |
| 🔴 | 0 / 100 | **C1PC03** | Banana-Thief | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/03_C1PC03_Banana-Thief) |
| 🔴 | 0 / 100 | **C1PC04** | Calculus survival | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/04_C1PC04_Calculus_survival) |
| 🔴 | 0 / 100 | **C1PC05** | Banana Mega-Trading | 1.500 seconds / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/05_C1PC05_Banana_Mega-Trading) |
| 🔴 | 0 / 100 | **C1PC06** | Banana Smuggling | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/06_C1PC06_Banana_Smuggling) |
| 🔴 | 0 / 100 | **C1PC07** | Narak Turing | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/07_C1PC07_Narak_Turing) |
| 🔴 | 0 / 100 | **C1PC08** | Pretender | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/08_C1PC08_Pretender) |
| 🔴 | 0 / 100 | **C1PC09** | Box | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/09_C1PC09_Box) |
| 🔴 | 0 / 100 | **C1PC10** | Charlie Takkie | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/10_C1PC10_Charlie_Takkie) |
| 🔴 | 0 / 100 | **C1PC11** | The Kuduy Bank | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/11_C1PC11_The_Kuduy_Bank) |
| 🔴 | 0 / 100 | **C1PC12** | Minesweeper | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/12_C1PC12_Minesweeper) |
| 🔴 | 0 / 100 | **C1PR01** | Test | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/13_C1PR01_Test) |
| 🔴 | 0 / 100 | **C1PR02** | KrisTerraFirmaGreg | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/14_C1PR02_KrisTerraFirmaGreg) |
| 🔴 | 0 / 100 | **C1PR03** | Binaemon DORA-Yaki Delirium | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/15_C1PR03_Binaemon_DORA-Yaki_Delirium) |
| 🔴 | 0 / 100 | **C1PR04** | Binaemon DORA-Yaki Delirium V2 | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/16_C1PR04_Binaemon_DORA-Yaki_Delirium_V2) |
| 🔴 | 0 / 100 | **C1PR05** | Uma_Musume | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/17_C1PR05_Uma_Musume) |
| 🔴 | 0 / 100 | **C1PR06** | Summarize | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/18_C1PR06_Summarize) |
| 🔴 | 0 / 100 | **C1PR07** | Box Box Box | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/19_C1PR07_Box_Box_Box) |
| 🔴 | 0 / 100 | **C1PR08** | WRP7 | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/20_C1PR08_WRP7) |
| 🔴 | 0 / 100 | **C1PR09** | Sebastian Vettel | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/21_C1PR09_Sebastian_Vettel) |
| 🔴 | 0 / 100 | **C1PR10** | Bossfight | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/22_C1PR10_Bossfight) |
| 🔴 | 0 / 100 | **C1PR11** | Funko Pop | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/23_C1PR11_Funko_Pop) |
| 🔴 | 0 / 100 | **C1PR12** | Racing Game | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/24_C1PR12_Racing_Game) |
| 🔴 | 0 / 100 | **C1PR13** | Food Stall | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/25_C1PR13_Food_Stall) |
| 🔴 | 0 / 100 | **C1PR14** | Restaurant | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/26_C1PR14_Restaurant) |
| 🔴 | 0 / 100 | **C1PR15** | Delivery | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/27_C1PR15_Delivery) |
| 🔴 | 0 / 100 | **C1PR16** | PrimeQ | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/28_C1PR16_PrimeQ) |
| 🔴 | 0 / 100 | **C1PR17** | PrimeSweepi-N | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/29_C1PR17_PrimeSweepi-N) |
| 🔴 | 0 / 100 | **C1PR18** | ClosingTime | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/30_C1PR18_ClosingTime) |
| 🔴 | 0 / 100 | **C1PE01** | C1PE01 - Constructor | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/31_C1PE01_Constructor) |
| 🔴 | 0 / 100 | **C1PE02** | C1PE02 - IPO | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/32_C1PE02_IPO) |
| 🔴 | 0 / 100 | **C1PE03** | C1PE03 - Mike Rock | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/33_C1PE03_Mike_Rock) |
| 🔴 | 0 / 100 | **C1PE04** | C1PE04 - Egypt | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/34_C1PE04_Egypt) |
| 🔴 | 0 / 100 | **C1PC13** | Guess the song | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/35_C1PC13_Guess_the_song) |
| 🔴 | 0 / 100 | **C1PC14** | Judgement | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/36_C1PC14_Judgement) |
<!-- SCORE:END -->

---

## 📝 สรุปที่เขียนเอง

| หัวข้อ | ไฟล์ |
|---|---|
| _(ยังไม่มี)_ | |
