# olympicComputer-camp1-2569 — เวิร์กสเปซค่าย 1 สอวน. คอม

เวิร์กสเปซส่วนตัวสำหรับค่าย 1 สอวน. คอมพิวเตอร์ ปี 2569
อยู่ใน `03_Academics/OCom_2026/Camp1/` — นี่คือ git repo เดียวในโฟลเดอร์นี้

## แผนผัง

| โฟลเดอร์ | เก็บอะไร |
|---|---|
| `docs/` | เอกสารค่าย: `syllabus/` กำหนดการ · `sheets/` สไลด์เรียน · `exams/` ตัวข้อสอบ |
| `problems/1_precamp/` | พรีแคมป์ — `cmu-grader/` (ชุด C1PC / C1PE / C1PR), `pretest/` (C1PE05–08 · นับเป็น contest `precamp` เดียวกัน), `codeforces/`, `programming.in.th/` |
| `problems/2_incamp/` | โจทย์ระหว่างค่าย — แยกตามช่วงที่เว็บแบ่ง: `1_first_half/` กับ `2_second_half/` (ในแต่ละอันมี `cmu-grader/` + `yrc-grader/`), `exam/ex1/`, `exam/ex2/`, `pretest/`, `sheets/` |
| `problems/3_endcamp/` | งานหลังค่าย |
| `notes/` | สรุป/cheatsheet ที่ผู้ใช้เขียนเอง |
| `tools/` | สคริปต์ช่วยงาน + `graders.txt` + `scores/` — ดู `tools/README.md` |

## กติกาในโฟลเดอร์นี้

- โฟลเดอร์โจทย์ 1 ข้อ = **`<ลำดับ>_<รหัสโจทย์>_<ชื่อโจทย์>`** เช่น
  `01_C1PC01_Alice_and_Bob` — ลำดับตามบรรทัดใน `tools/scores/*.txt` เพื่อให้
  เรียงตามลำดับโจทย์ ไม่ใช่ตามรหัส (C1PC → C1PE → C1PR)
  **ไฟล์ข้างในยังชื่อ `<ID>.cpp` + `<ID>.pdf`** ไม่ต่อชื่อโจทย์ เพราะ grader
  บังคับชื่อไฟล์ตอน submit (คอลัมน์ Files) — โฟลเดอร์กับไฟล์ชื่อไม่เหมือนกันโดยตั้งใจ
  ถ้า Files กับรหัส Task ไม่ตรงกัน (เกิดกับ YRC) **ให้ยึด Files** เช่น
  `01_BobInverse/Bob_Inverse.cpp` · `05_IF02_If_esle_ระดับ_2/Y_IF_ELSE_02.pdf`
  เปลี่ยนชื่อโฟลเดอร์ด้วย `python3 tools/folder_names.py --apply` อย่า rename มือ
- ข้อที่ได้ 100/100 เต็ม จะถูก **ย้ายเข้า `_done/`** ของ grader นั้นด้วย
  `sync_done.py` — อย่าย้ายมือ
- ตารางคะแนนใน `README.md` อยู่ระหว่าง `<!-- SCORE:START -->` / `<!-- SCORE:END -->`
  **สร้างอัตโนมัติจาก `tools/scores/*.txt` — ห้ามแก้มือ** ของจริงคือไฟล์ในนั้น
  หนึ่งไฟล์ต่อหนึ่ง contest (ชื่อไฟล์ต้องตรงกับ `tools/graders.txt`)
- **โจทย์แบบฟังก์ชัน** (เว็บให้ `grader.cpp` + `.h` มา เราเขียนแค่ตัวฟังก์ชัน) แยกของสองฝั่งให้ชัด
  ```
  <โฟลเดอร์โจทย์>/
  ├── <Files>.cpp    ← ไฟล์ที่เราแก้+ส่ง — อยู่ชั้นบนสุดเสมอ ชื่อตามคอลัมน์ Files
  ├── <ID>.pdf
  ├── provided/      ← ของที่ grader ให้มา ทั้งก้อน (grader.cpp · <ID>.h · compile*.sh|bat · sample/)
  ├── local/         ← ไฟล์ซ้อม/เวอร์ชันลองของเรา (ไม่ได้ส่ง · commit ไว้ด้วย)
  └── .run/          ← ของที่คอมไพล์/รันออกมา (gitignore)
  ```
  `provided/` = **ห้ามแก้** (ตั้ง read-only ใน `.vscode/settings.json` แล้ว) · PDF ตัด suffix
  ไทยทิ้ง (`C1T43 (th2).pdf` → `C1T43.pdf`) · zip ของ grader ที่แตกแล้วให้ `git rm` ทิ้ง
- **รันโจทย์ด้วย `python3 tools/run.py <รหัส>`** — สคริปต์กลางตัวเดียว รู้จักทั้งสองทรง
  (`--file` · `--sample N` · `--all-samples` · `-b` · `--asan` · `--list` · `--clean`)
  **ห้ามใส่ `&&` / `<` / `>` ลงใน `args` ของ `.vscode/tasks.json`** — task ใช้ `type: process`
  ไม่ผ่าน shell เพื่อให้ใช้ได้ทั้ง fish/bash/cmd/PowerShell · `isDefault: true` มีได้ตัวเดียว
- สไลด์ตั้งชื่อ `YYYYMMDD_AM_Topic` / `YYYYMMDD_PM_Topic`
- `template.cpp` คือโค้ดตั้งต้นของทุกข้อ — แก้ได้ตามใจ ผู้ใช้เป็นเจ้าของสไตล์นี้

## เรื่องที่ต้องระวัง

- **โจทย์โหลดมือจากเว็บ grader — ไม่มีสคริปต์ดึงอัตโนมัติ และไม่มีรหัสอะไรเก็บใน
  repo นี้เลย** อย่าเสนอให้เขียนสคริปต์ล็อกอิน/สแครป grader ผู้ใช้เลือกโหลดเองแล้ว
- เว็บ `https://www.cs.science.cmu.ac.th/compo/` มีหลาย contest (พรีแคมป์ ·
  ครึ่งแรก · ครึ่งหลัง · pretest · สอบ 2 ครั้ง) เป็น CMS/Zebra คนละตัวกัน
  ทะเบียนอยู่ที่ `tools/graders.txt` — เพิ่ม contest = เพิ่ม 1 บรรทัด ไม่ต้องแก้สคริปต์
- `tools/scores/<ชื่อ>.txt` คือข้อความที่ก็อปมาจากหน้าเว็บ grader — มีชื่อโจทย์
  ภาษาไทย ระวัง encoding (UTF-8) ตอนเขียนสคริปต์อ่าน
- ชื่อไฟล์/โฟลเดอร์ภาษาไทยมีอยู่จริงในไฟล์คะแนนและ PDF — quote ใน shell เสมอ
- ของใน `docs/` กับ PDF โจทย์เป็น**หลักฐานผลงาน** — ห้ามลบ/ย้ายโดยไม่ถาม
- ไฟล์ที่คอมไพล์ (`.out`, `.exe`) ไม่ขึ้น git · ไฟล์รันชั่วคราวอยู่ `.run/` (gitignored)
  ⚠️ `.gitignore` มี `*.out` ซึ่งกลืน `sample/*.out` / `testcases/*.out` ที่**ต้องขึ้น git**
  (เป็นคำตอบที่ grader ให้มา) — กฎกลับด้าน (`!...`) จึงต้องวาง**ท้ายไฟล์** เพราะ git
  ใช้กฎตัวท้ายสุดที่ match · เช็คด้วย `git add -n <path>` ไม่ใช่ `git check-ignore -v`
  (ตัวหลังพิมพ์กฎที่ match รวม negation ทำให้นึกว่าถูก ignore)
  ⚠️ และ `.gitattributes` **ต้องไม่มี `*.out binary`** — ตัว binary ที่คอมไพล์ออกมาถูก
  `.gitignore` กันอยู่แล้ว แต่ `sample/*.out` ที่ต้อง diff ได้จะโดนหางเลขไปด้วย
  (`binary` = `-diff -merge -text` → git มองเป็น binary แล้ว diff ไม่ออก)
- **เครื่องนี้ยังไม่มี `gdb`** — F5 ใน `.vscode/launch.json` ใช้ไม่ได้จนกว่าจะ `sudo apt install gdb`
  ระหว่างนี้ใช้ task `C++: compile with AddressSanitizer` หรือ `run.py --asan` แทน
  · F5 ต้องเปิดไฟล์ `.cpp` **ชั้นบนสุดของโฟลเดอร์โจทย์** (program ใช้ `${fileDirname}`)

## โค้ดจริงอยู่ที่อื่น

งาน competitive programming จริงของผู้ใช้อยู่ใน
`01_Development/Comp_Prog_TOI` — ที่นี่เก็บโจทย์/สไลด์/สรุปเป็นหลัก
แต่โค้ดที่เขียนในค่ายให้เก็บคู่กับโจทย์ใน `problems/` เลย
