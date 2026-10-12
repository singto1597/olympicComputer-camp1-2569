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
├── .vscode/             ← tasks.json (ปุ่ม build/run) + launch.json (F5 debug)
│
├── docs/                ← เอกสารค่าย (ที่ไม่ใช่โจทย์)
│   ├── syllabus/          กำหนดการ, syllabus, เอกสารสมัคร
│   ├── sheets/            สไลด์/ชีทเรียน  →  YYYYMMDD_AM_Topic/
│   └── exams/             ตัวข้อสอบ pretest / midterm / final
│
├── problems/            ← โจทย์ทั้งหมด
│   ├── 1_precamp/         พรีแคมป์ (ก่อนเข้าค่าย)
│   │   ├── cmu-grader/      + _done/  ← 01_C1PC01_Alice_and_Bob/ ฯลฯ
│   │   ├── pretest/         Pretest ก่อนแคมป์ — 37..40_C1PE05..08/
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
| ของที่ grader ให้มา (โจทย์ฟังก์ชัน) | ซ้อนใน **`provided/`** — ห้ามแก้ | `provided/grader.cpp`, `provided/C1T43.h` |
| ไฟล์ซ้อม/เวอร์ชันลองของเรา (ไม่ได้ส่ง) | ซ้อนใน **`local/`** | `local/C1T38_localtestnew.cpp` |
| ของที่คอมไพล์/รันออกมา | **`.run/`** (gitignore) | `.run/bin`, `.run/input.in`, `.run/output.txt` |

- **โฟลเดอร์มีเลขนำหน้า** เพราะลำดับโจทย์บนเว็บไม่เหมือนลำดับรหัส (เว็บเรียง C1PC → C1PR →
  C1PE แต่เรียงตามตัวอักษรได้ C1PC → C1PE → C1PR) เลขลำดับมาจากบรรทัดใน
  `tools/scores/*.txt` — ตัวเดียวกับตารางคะแนนข้างล่าง
- **ไฟล์ข้างในยังเป็นรหัสเปล่า ๆ** เพราะ grader บังคับชื่อไฟล์ตอน submit (คอลัมน์
  Files = `C1PC01[.cpp|.c]`) ถ้าใส่ชื่อโจทย์ไว้ต้อง rename กลับก่อนส่งทุกครั้ง
  โฟลเดอร์กับไฟล์จึงชื่อไม่เหมือนกันโดยตั้งใจ
- **ยึดคอลัมน์ Files ไม่ใช่รหัส Task** — บาง grader สองอย่างนี้ไม่ตรงกัน โฟลเดอร์ใช้
  รหัส Task ส่วนไฟล์ข้างในใช้ชื่อที่ grader บังคับ เช่น YRC `BobInverse` → `Bob_Inverse.cpp`
  · `IF02` → `Y_IF_ELSE_02.pdf`
- เลขนำหน้ามีแค่ระดับ phase (`1_precamp` → `2_incamp` → `3_endcamp`) เพราะต้องเรียงตามเวลา
- **โจทย์แบบฟังก์ชัน** (เว็บให้ `grader.cpp` + `.h` มา เราเขียนแค่ตัวฟังก์ชัน) — ของที่ grader
  ให้มาอยู่ใน `provided/` ทั้งก้อน ส่วนไฟล์ที่เราแก้+ส่งอยู่ **ชั้นบนสุด** ของโฟลเดอร์โจทย์
  ชื่อตามคอลัมน์ Files เหมือนเดิม (`C1T43.cpp` · `exchange.cpp` · `find_max_and_min.cpp` ·
  `n_pens.cpp`) · `provided/` ถูกตั้งเป็น read-only ใน VS Code (`.vscode/settings.json`)
- ไฟล์ที่คอมไพล์/รันออกมาไปที่ **`.run/`** ของโฟลเดอร์โจทย์ (ไม่ใช่ราก repo) ไม่ขึ้น git
  — ดู `.gitignore` · `sample/*.out` กับ `testcases/*.out` **ขึ้น git** (เป็นคำตอบที่ grader ให้มา)

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

# 3. เขียนโค้ด — แล้ว build + run ด้วยคำสั่งเดียว (จำรหัสโจทย์ ไม่ต้อง cd)
python3 tools/run.py C1PR01           # คอมไพล์ + รัน (ใช้ .run/input.in ถ้ามี)
python3 tools/run.py C1T43 --sample 1 # โจทย์ฟังก์ชันก็ใช้คำสั่งเดียวกัน
python3 tools/run.py C1T43 --all-samples
#    หรือเปิดไฟล์แล้วกด Ctrl+Shift+B ใน VS Code (เรียกตัวเดียวกันนี้)

# 4. ก็อปตารางคะแนนจากหน้าเว็บ → tools/scores/precamp.txt แล้วรัน
python3 tools/sync_done.py     # ย้ายข้อที่ได้ 100 → _done/ และอัปเดตตารางข้างล่าง
```

รหัสโจทย์ไม่ซ้ำกันระหว่างค่าย — `sync_done.py` หาโฟลเดอร์เจอเองโดยไม่ต้องบอกว่า
ข้อไหนอยู่ช่วงไหน ส่วนรายชื่อ contest ทั้งหมดดูที่ `tools/graders.txt`

รายละเอียดของ `run.py` (โหมดทั้งหมด · โครง `provided/` · วิธี debug) อยู่ที่
[`tools/README.md`](tools/README.md)

---

## 🏆 ตารางคะแนน

> สร้างอัตโนมัติโดย `python3 tools/sync_done.py` จาก `tools/scores/*.txt` — อย่าแก้มือ

<!-- SCORE:START -->
### พรีแคมป์ — ได้เต็ม **18 / 40** ข้อ

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
| 🟢 | 100 / 100 | **C1PR01** | Test | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/13_C1PR01_Test) |
| 🟢 | 100 / 100 | **C1PR02** | KrisTerraFirmaGreg | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/14_C1PR02_KrisTerraFirmaGreg) |
| 🟢 | 100 / 100 | **C1PR03** | Binaemon DORA-Yaki Delirium | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/15_C1PR03_Binaemon_DORA-Yaki_Delirium) |
| 🟢 | 100 / 100 | **C1PR04** | Binaemon DORA-Yaki Delirium V2 | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/16_C1PR04_Binaemon_DORA-Yaki_Delirium_V2) |
| 🟢 | 100 / 100 | **C1PR05** | Uma_Musume | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/17_C1PR05_Uma_Musume) |
| 🟢 | 100 / 100 | **C1PR06** | Summarize | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/18_C1PR06_Summarize) |
| 🟢 | 100 / 100 | **C1PR07** | Box Box Box | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/19_C1PR07_Box_Box_Box) |
| 🟢 | 100 / 100 | **C1PR08** | WRP7 | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/20_C1PR08_WRP7) |
| 🟢 | 100 / 100 | **C1PR09** | Sebastian Vettel | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/21_C1PR09_Sebastian_Vettel) |
| 🟢 | 100 / 100 | **C1PR10** | Bossfight | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/22_C1PR10_Bossfight) |
| 🟢 | 100 / 100 | **C1PR11** | Funko Pop | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/23_C1PR11_Funko_Pop) |
| 🟢 | 100 / 100 | **C1PR12** | Racing Game | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/24_C1PR12_Racing_Game) |
| 🟢 | 100 / 100 | **C1PR13** | Food Stall | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/25_C1PR13_Food_Stall) |
| 🟢 | 100 / 100 | **C1PR14** | Restaurant | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/26_C1PR14_Restaurant) |
| 🟢 | 100 / 100 | **C1PR15** | Delivery | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/27_C1PR15_Delivery) |
| 🟢 | 100 / 100 | **C1PR16** | PrimeQ | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/28_C1PR16_PrimeQ) |
| 🟢 | 100 / 100 | **C1PR17** | PrimeSweepi-N | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/29_C1PR17_PrimeSweepi-N) |
| 🟢 | 100 / 100 | **C1PR18** | ClosingTime | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/_done/30_C1PR18_ClosingTime) |
| 🔴 | 0 / 100 | **C1PE01** | C1PE01 - Constructor | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/31_C1PE01_Constructor) |
| 🔴 | 0 / 100 | **C1PE02** | C1PE02 - IPO | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/32_C1PE02_IPO) |
| 🔴 | 0 / 100 | **C1PE03** | C1PE03 - Mike Rock | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/33_C1PE03_Mike_Rock) |
| 🔴 | 0 / 100 | **C1PE04** | C1PE04 - Egypt | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/34_C1PE04_Egypt) |
| 🔴 | 0 / 100 | **C1PC13** | Guess the song | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/35_C1PC13_Guess_the_song) |
| 🔴 | 0 / 100 | **C1PC14** | Judgement | 1.000 second / 8.00 MiB | [📁](problems/1_precamp/cmu-grader/36_C1PC14_Judgement) |
| 🔴 | 0 / 100 | **C1PE05** | การอยู่รอดของแท็กกับฟิสิกส์ (Physics survival) | N/A / N/A | [📁](problems/1_precamp/pretest/37_C1PE05_การอยู่รอดของแท็กกับฟิสิกส์_(Physics_survival)) |
| 🔴 | 0 / 100 | **C1PE06** | Sakiko Question | 1.000 second / 256 MiB | [📁](problems/1_precamp/pretest/38_C1PE06_Sakiko_Question) |
| 🔴 | 0 / 100 | **C1PE07** | 01 Game | N/A / N/A | [📁](problems/1_precamp/pretest/39_C1PE07_01_Game) |
| 🔴 | 0 / 100 | **C1PE08** | Game Of Life | 3.000 seconds / 64.0 MiB | [📁](problems/1_precamp/pretest/40_C1PE08_Game_Of_Life) |

### ค่าย 1 ครึ่งแรก — ได้เต็ม **48 / 54** ข้อ

| สถานะ | คะแนน | Task | ชื่อโจทย์ | ลิมิต | ไฟล์ | หมายเหตุ |
| :---: | :---: | :--- | :--- | :---: | :---: | :--- |
| 🟢 | 100 / 100 | **C1C0_ADD** | C1C0_ADD | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/01_C1C0_ADD) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y3** | สอวน. คอมพิวเตอร์ ค่าย 1 | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/02_C1Y3_สอวน._คอมพิวเตอร์_ค่าย_1) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N1** | หาค่าเฉลี่ยตัวเลข 5 จำนวน | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/03_C1N1_หาค่าเฉลี่ยตัวเลข_5_จำนวน) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N2** | หาค่าดัชนีมวลกาย BMI | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/04_C1N2_หาค่าดัชนีมวลกาย_BMI) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y4** | เกรดอลวน | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/05_C1Y4_เกรดอลวน) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y5** | ส่วนลด | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/06_C1Y5_ส่วนลด) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y6** | คู่หรือคี่ใครแน่จริง | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/07_C1Y6_คู่หรือคี่ใครแน่จริง) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N5** | แปลงเวลา | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/08_C1N5_แปลงเวลา) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N6** | แลกเงิน | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/09_C1N6_แลกเงิน) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y7** | วนพิมพ์ตัวเลขตามขอบเขตที่กำหนด | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/10_C1Y7_วนพิมพ์ตัวเลขตามขอบเขตที่กำหนด) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y9** | คู่หรือคี่ใครแน่จริง | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/11_C1Y9_คู่หรือคี่ใครแน่จริง) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N7** | ตัวเลขที่หารด้วย 3 และ 5 ลงตัว | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/12_C1N7_ตัวเลขที่หารด้วย_3_และ_5_ลงตัว) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N8** | เล่นกับอักขระ | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/13_C1N8_เล่นกับอักขระ) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N9** | ตัดเกรด | 1.000 second / 4.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/14_C1N9_ตัดเกรด) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T1** | Banana Shop | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/15_C1T1_Banana_Shop) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T2** | หงุดหงิดเพราะติดโจทย์ | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/16_C1T2_หงุดหงิดเพราะติดโจทย์) |  |
| 🟢 | 100 / 100 | **C1T3** | เครื่องคอมเจ้าปัญหา | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/17_C1T3_เครื่องคอมเจ้าปัญหา) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T4** | สูตรน้ำอร่อย | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/18_C1T4_สูตรน้ำอร่อย) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T5** | โรงแรมใจกลางเมืองน่านแห่งหนึ่ง | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/19_C1T5_โรงแรมใจกลางเมืองน่านแห่งหนึ่ง) | 🕰️ 100 เก่า |
| 🔴 | 0 / 100 | **C1T6** | TCS | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/20_C1T6_TCS) |  |
| 🟢 | 100 / 100 | **C1T7** | เทค่าย | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/21_C1T7_เทค่าย) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T8** | ปริศนาแห่งโลกใหม่ | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/22_C1T8_ปริศนาแห่งโลกใหม่) |  |
| 🟢 | 100 / 100 | **C1T9** | กองเรือแห่งโลกใหม | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/23_C1T9_กองเรือแห่งโลกใหม) |  |
| 🟢 | 100 / 100 | **C1T10** | ธงแห่งโลกใหม | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/24_C1T10_ธงแห่งโลกใหม) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T11** | ประตูมิติ | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/25_C1T11_ประตูมิติ) |  |
| 🟢 | 100 / 100 | **C1T12** | ขโมยต้นไม้ | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/26_C1T12_ขโมยต้นไม้) |  |
| 🔴 | 0 / 100 | **C1T13** | รหัสลับแห่งแกรนด์ไลน์ | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/27_C1T13_รหัสลับแห่งแกรนด์ไลน์) |  |
| 🟢 | 100 / 100 | **C1T14** | เส้นทางที่ล้ำค่าที่สุด | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/28_C1T14_เส้นทางที่ล้ำค่าที่สุด) |  |
| 🟢 | 100 / 100 | **C1T15** | ค้นหาห้องในโรงแรมใจกลางเมืองน่าน | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/29_C1T15_ค้นหาห้องในโรงแรมใจกลางเมืองน่าน) |  |
| 🟢 | 100 / 100 | **C1Y10** | ตำแหน่งของเลขที่น่าสนใจ | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/30_C1Y10_ตำแหน่งของเลขที่น่าสนใจ) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y11** | เกมทศกัณฐ์ | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/31_C1Y11_เกมทศกัณฐ์) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y12** | พิมพ์ตัวเลขย้อนกลับ | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/32_C1Y12_พิมพ์ตัวเลขย้อนกลับ) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y13** | รหัสซีซาร์ | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/33_C1Y13_รหัสซีซาร์) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y14** | palindrome | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/34_C1Y14_palindrome) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y15** | การข้อมูลสตริง | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/35_C1Y15_การข้อมูลสตริง) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y17** | แปลงเลขฐาน 2 | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/36_C1Y17_แปลงเลขฐาน_2) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y18** | คำนวณค่า factorial | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/37_C1Y18_คำนวณค่า_factorial) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y19** | คำนวณเลขยกกำลัง | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/38_C1Y19_คำนวณเลขยกกำลัง) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N10** | Max Even Odd | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/39_C1N10_Max_Even_Odd) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N11** | Max Min Average | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/40_C1N11_Max_Min_Average) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N13** | นับอักขระ | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/41_C1N13_นับอักขระ) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T16** | verticalSpiral | 1.000 second / 512 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/42_C1T16_verticalSpiral) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T17** | HolyWaterForDaWays | 1.000 second / 256 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/43_C1T17_HolyWaterForDaWays) |  |
| 🔴 | 0 / 100 | **C1T18** | Dash-separated Numbers | 1.000 second / 256 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/44_C1T18_Dash-separated_Numbers) |  |
| 🟢 | 100 / 100 | **C1T36** | tiktok | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/45_C1T36_tiktok) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1C00** | Determinant | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/46_C1C00_Determinant) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1C01** | Last K Digits | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/47_C1C01_Last_K_Digits) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **O24C1P1** | Hipporchestra | 0.040 seconds / 64.0 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/48_O24C1P1_Hipporchestra) |  |
| 🟢 | 100 / 100 | **O24C1P2** | Heros of tn square | 1.000 second / 256 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/49_O24C1P2_Heros_of_tn_square) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **O24C1P3** | Prize | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/50_O24C1P3_Prize) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **O24C1P4** | The Winner | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/_done/51_O24C1P4_The_Winner) |  |
| 🔴 | 0 / 100 | **C1T40** | Kuromi’s Bad Mood | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/52_C1T40_Kuromi’s_Bad_Mood) |  |
| 🔴 | 0 / 100 | **C1T41** | Banana Syndicate | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/53_C1T41_Banana_Syndicate) |  |
| 🔴 | 0 / 100 | **C1T42** | Banana 3-factor | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/cmu-grader/54_C1T42_Banana_3-factor) |  |

> 🕰️ 100 เก่า = ได้เต็มจากการส่งโค้ดเก่าซ้ำ (โค้ดจากปี 68 · ไม่ได้เขียนใหม่ปีนี้) — 38 ข้อ

### ค่าย 1 ครึ่งแรก (YRC) — ได้เต็ม **0 / 17** ข้อ

| สถานะ | คะแนน | Task | ชื่อโจทย์ | ลิมิต | ไฟล์ |
| :---: | :---: | :--- | :--- | :---: | :---: |
| 🔴 | 0 / 100 | **BobInverse** | BobInverse | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/yrc-grader/01_BobInverse) |
| 🔴 | 0 / 100 | **BobInWonderland** | BobInWonderland | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/yrc-grader/02_BobInWonderland) |
| 🔴 | 0 / 100 | **JJFourier** | JJFourier | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/yrc-grader/03_JJFourier) |
| 🔴 | 0 / 100 | **Sausage** | Sausage | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/1_first_half/yrc-grader/04_Sausage) |
| 🔴 | 0 / 100 | **IF01** | If_else ระดับ 1 | N/A / N/A | [📁](problems/2_incamp/1_first_half/yrc-grader/05_IF01_If_else_ระดับ_1) |
| 🔴 | 0 / 100 | **IF02** | If_esle ระดับ 2 | N/A / N/A | [📁](problems/2_incamp/1_first_half/yrc-grader/06_IF02_If_esle_ระดับ_2) |
| 🔴 | 0 / 100 | **IF03** | If_else_ระดับ 3 | N/A / N/A | [📁](problems/2_incamp/1_first_half/yrc-grader/07_IF03_If_else_ระดับ_3) |
| 🔴 | 0 / 100 | **IF04** | Y_IF_ELSE_04 | N/A / N/A | [📁](problems/2_incamp/1_first_half/yrc-grader/08_IF04_Y_IF_ELSE_04) |
| 🔴 | 0 / 100 | **IF05** | Y_IF_ELSE_05 | N/A / N/A | [📁](problems/2_incamp/1_first_half/yrc-grader/09_IF05_Y_IF_ELSE_05) |
| 🔴 | 0 / 100 | **IF06** | Y_IF_ELSE_06 | N/A / N/A | [📁](problems/2_incamp/1_first_half/yrc-grader/10_IF06_Y_IF_ELSE_06) |
| 🔴 | 0 / 100 | **L01** | Loop ระดับ 1 | 1.000 second / 64.0 MiB | — |
| 🔴 | 0 / 100 | **L02** | Loop ระดับ 2 | 1.000 second / 64.0 MiB | — |
| 🔴 | 0 / 100 | **L03** | Loop ระดับ 3 | 1.000 second / 64.0 MiB | — |
| 🔴 | 0 / 100 | **L04** | Loop ระดับ 4 | 1.000 second / 64.0 MiB | — |
| 🔴 | 0 / 100 | **L05** | Loop ระดับ 5 | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/yrc-grader/15_L05_Loop_ระดับ_5) |
| 🔴 | 0 / 100 | **CanYouDeliverInTime** | CanYouDeliverInTime | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/yrc-grader/16_CanYouDeliverInTime) |
| 🔴 | 0 / 100 | **DecodingImage** | DecodingImage | 1.000 second / 64.0 MiB | [📁](problems/2_incamp/1_first_half/yrc-grader/17_DecodingImage) |

### ค่าย 1 ครึ่งหลัง — ได้เต็ม **23 / 57** ข้อ

| สถานะ | คะแนน | Task | ชื่อโจทย์ | ลิมิต | ไฟล์ | หมายเหตุ |
| :---: | :---: | :--- | :--- | :---: | :---: | :--- |
| 🟢 | 100 / 100 | **C1N14** | Pointer : StRiNg | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/01_C1N14_Pointer_StRiNg) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N15** | Pointer : Sum_Even_Number | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/02_C1N15_Pointer_Sum_Even_Number) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N17** | MAX MIN | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/03_C1N17_MAX_MIN) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N18** | Number to Text | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/04_C1N18_Number_to_Text) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N19** | Digit count | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/05_C1N19_Digit_count) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N20** | Square | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/06_C1N20_Square) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N21** | X number pattern | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/07_C1N21_X_number_pattern) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N22** | Parking | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/08_C1N22_Parking) | 🕰️ 100 เก่า |
| 🟡 | 50 / 100 | **C1N25** | Selection Sort | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/09_C1N25_Selection_Sort) |  |
| 🟢 | 100 / 100 | **C1N26** | Bubble Sort | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/10_C1N26_Bubble_Sort) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N27** | BOX STACK | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/11_C1N27_BOX_STACK) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1N28** | findMaxAndMin | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/12_C1N28_findMaxAndMin) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y20** | SUM(Pointer) | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/13_C1Y20_SUM(Pointer)) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y21** | SWAP(pointer) | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/14_C1Y21_SWAP(pointer)) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y22** | Search me(Pointer) | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/15_C1Y22_Search_me(Pointer)) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y23** | แบบฝึกหัด แบบมีทางเลือก | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/16_C1Y23_แบบฝึกหัด_แบบมีทางเลือก) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y24** | GCD | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/17_C1Y24_GCD) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y25** | ค้นหา | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/18_C1Y25_ค้นหา) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1Y26** | Peak Elements | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/19_C1Y26_Peak_Elements) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **C1T19** | มูลช้าง | 2.000 seconds / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/20_C1T19_มูลช้าง) | 🕰️ 100 เก่า |
| 🔴 | 0 / 100 | **C1T20** | ตาเดินเกมเศรษฐี | 2.000 seconds / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/21_C1T20_ตาเดินเกมเศรษฐี) |  |
| 🔴 | 0 / 100 | **C1T21** | กล่องของเล่นของ TA | 1.000 second / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/22_C1T21_กล่องของเล่นของ_TA) |  |
| 🔴 | 0 / 100 | **C1T22** | Distributed Computing | 1.000 second / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/23_C1T22_Distributed_Computing) |  |
| 🔴 | 0 / 100 | **C1T23** | Matrix | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/24_C1T23_Matrix) |  |
| 🔴 | 0 / 100 | **C1T24** | ผลไม้อลเวง | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/25_C1T24_ผลไม้อลเวง) |  |
| 🔴 | 0 / 100 | **C1T25** | วาดรูปอลเวง | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/26_C1T25_วาดรูปอลเวง) |  |
| 🔴 | 0 / 100 | **C1T26** | Homework | 1.000 second / 128 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/27_C1T26_Homework) |  |
| 🔴 | 0 / 100 | **C1T27** | Into Dungeons | 2.000 seconds / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/28_C1T27_Into_Dungeons) |  |
| 🟢 | 100 / 100 | **C1T28** | No Moon | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/29_C1T28_No_Moon) | 🕰️ 100 เก่า |
| 🔴 | 0 / 100 | **C1T29** | Rainy Season | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/30_C1T29_Rainy_Season) |  |
| 🔴 | 0 / 100 | **C1T30** | Banana Shiping | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/31_C1T30_Banana_Shiping) |  |
| 🔴 | 0 / 100 | **C1T31** | Tired | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/32_C1T31_Tired) |  |
| 🔴 | 0 / 100 | **C1T32** | Banana Prime | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/33_C1T32_Banana_Prime) |  |
| 🔴 | 0 / 100 | **C1T33** | Stealing Banana | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/34_C1T33_Stealing_Banana) |  |
| 🔴 | 0 / 100 | **C1T34** | House of card | 5.000 seconds / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/35_C1T34_House_of_card) |  |
| 🔴 | 0 / 100 | **C1T35** | ป้อMอลเวง | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/36_C1T35_ป้อMอลเวง) |  |
| 🔴 | 0 / 100 | **C1T37** | whatisrsa | 4.000 seconds / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/37_C1T37_whatisrsa) |  |
| 🟢 | 100 / 100 | **C1T38** | เก็บข้อมูลผจญภัย | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/38_C1T38_เก็บข้อมูลผจญภัย) | 🕰️ 100 เก่า |
| 🔴 | 0 / 100 | **C1T39** | ตลาดของนายเก้า | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/39_C1T39_ตลาดของนายเก้า) |  |
| 🔴 | 0 / 100 | **O23C1P1** | สะสมเหรียญ | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/40_O23C1P1_สะสมเหรียญ) |  |
| 🔴 | 0 / 100 | **O23C1P2** | All Blue | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/41_O23C1P2_All_Blue) |  |
| 🔴 | 0 / 100 | **O23C1P3** | แข่งขันชิงรางวัล | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/42_O23C1P3_แข่งขันชิงรางวัล) |  |
| 🟢 | 100 / 100 | **O23C1P4** | Word Transformation | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/43_O23C1P4_Word_Transformation) | 🕰️ 100 เก่า |
| 🟢 | 100 / 100 | **O23C1P5** | สลับตู้โดยสาร | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/_done/44_O23C1P5_สลับตู้โดยสาร) | 🕰️ 100 เก่า |
| 🔴 | 0 / 100 | **O23C1P6** | ETS | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/45_O23C1P6_ETS) |  |
| 🔴 | 0 / 100 | **O24C1P5** | Moo Deng Kidnapped! | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/46_O24C1P5_Moo_Deng_Kidnapped!) |  |
| 🔴 | 0 / 100 | **O24C1P6** | Phakwan Power Plant | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/47_O24C1P6_Phakwan_Power_Plant) |  |
| 🔴 | 0 / 100 | **O24C1P7** | Zoo Tour | 1.000 second / 16.0 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/48_O24C1P7_Zoo_Tour) |  |
| 🔴 | 0 / 100 | **O24C1P8** | Jelly In A Bottle | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/49_O24C1P8_Jelly_In_A_Bottle) |  |
| 🔴 | 0 / 100 | **C1T43** | Kuromi_s Energy | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/50_C1T43_Kuromi_s_Energy) |  |
| 🔴 | 0 / 100 | **C1T44** | Upside-down Hachiware-2 | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/51_C1T44_Upside-down_Hachiware-2) |  |
| 🔴 | 0 / 100 | **C1T45** | Linked List Operations | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/52_C1T45_Linked_List_Operations) |  |
| 🔴 | 0 / 100 | **C1C02** | Exchange | 1.000 second / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/53_C1C02_Exchange) |  |
| 🔴 | 0 / 100 | **C1C03** | findMaxAndMin | 1.000 second / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/54_C1C03_findMaxAndMin) |  |
| 🔴 | 0 / 100 | **C1C04** | N Pens | 1.000 second / 256 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/55_C1C04_N_Pens) |  |
| 🔴 | 0 / 100 | **C1T46** | Stack using array | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/56_C1T46_Stack_using_array) |  |
| 🔴 | 0 / 100 | **C1T47** | Queue using array | 1.000 second / 8.00 MiB | [📁](problems/2_incamp/2_second_half/cmu-grader/57_C1T47_Queue_using_array) |  |

> 🕰️ 100 เก่า = ได้เต็มจากการส่งโค้ดเก่าซ้ำ (โค้ดจากปี 68 · ไม่ได้เขียนใหม่ปีนี้) — 23 ข้อ
<!-- SCORE:END -->

---

## 📝 สรุปที่เขียนเอง

| หัวข้อ | ไฟล์ |
|---|---|
| _(ยังไม่มี)_ | |
