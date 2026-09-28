# olympicComputer-camp1-2569 — เวิร์กสเปซค่าย 1 สอวน. คอม

เวิร์กสเปซส่วนตัวสำหรับค่าย 1 สอวน. คอมพิวเตอร์ ปี 2569
อยู่ใน `03_Academics/OCom_2026/Camp1/` — นี่คือ git repo เดียวในโฟลเดอร์นี้

## แผนผัง

| โฟลเดอร์ | เก็บอะไร |
|---|---|
| `docs/` | เอกสารค่าย: `syllabus/` กำหนดการ · `sheets/` สไลด์เรียน · `exams/` ตัวข้อสอบ |
| `problems/1_precamp/` | โจทย์ทำก่อนเข้าค่าย (`codeforces/`, `programming.in.th/`) |
| `problems/2_incamp/` | โจทย์ระหว่างค่าย (`cmu-grader/`, `yrc-grader/`, `pretest/`, `exam/`, `sheets/`) |
| `problems/3_endcamp/` | งานหลังค่าย |
| `notes/` | สรุป/cheatsheet ที่ผู้ใช้เขียนเอง |
| `tools/` | สคริปต์ช่วยงาน + `score.txt` + `secrets.fish` (gitignored) |

## กติกาในโฟลเดอร์นี้

- โฟลเดอร์โจทย์ 1 ข้อ = **รหัสโจทย์ตรงกับบน grader** (`C1P01`) ข้างในมี
  `<ID>.cpp` + `<ID>.pdf` — ไม่ใส่เลขนำหน้าโจทย์
- ข้อที่ได้ 100/100 เต็ม จะถูก **ย้ายเข้า `_done/`** ของ grader นั้นด้วย
  `sync_done.py` — อย่าย้ายมือ
- ตารางคะแนนใน `README.md` อยู่ระหว่าง `<!-- SCORE:START -->` / `<!-- SCORE:END -->`
  **สร้างจาก `tools/score.txt` อัตโนมัติ — ห้ามแก้มือ** ของจริงคือ `score.txt`
- สไลด์ตั้งชื่อ `YYYYMMDD_AM_Topic` / `YYYYMMDD_PM_Topic`
- `template.cpp` คือโค้ดตั้งต้นของทุกข้อ — แก้ได้ตามใจ ผู้ใช้เป็นเจ้าของสไตล์นี้

## เรื่องที่ต้องระวัง

- **`tools/secrets.fish` มี cookie ของ grader — ห้าม cat ห้าม commit**
  ถ้าต้องแก้ให้บอกผู้ใช้ทำเอง
- `tools/score.txt` เป็นข้อความที่ก็อปมาจากหน้าเว็บ grader — มีชื่อโจทย์ภาษาไทย
  ระวัง encoding (UTF-8) ตอนเขียนสคริปต์อ่าน
- ชื่อไฟล์/โฟลเดอร์ภาษาไทยมีอยู่จริงใน `score.txt` และ PDF — quote ใน shell เสมอ
- ของใน `docs/` กับ PDF โจทย์เป็น**หลักฐานผลงาน** — ห้ามลบ/ย้ายโดยไม่ถาม
- ไฟล์ที่คอมไพล์ (`.out`, `.exe`) ไม่ขึ้น git · ไฟล์รันชั่วคราวอยู่ `.run/` (gitignored)

## โค้ดจริงอยู่ที่อื่น

งาน competitive programming จริงของผู้ใช้อยู่ใน
`01_Development/Comp_Prog_TOI` — ที่นี่เก็บโจทย์/สไลด์/สรุปเป็นหลัก
แต่โค้ดที่เขียนในค่ายให้เก็บคู่กับโจทย์ใน `problems/` เลย
