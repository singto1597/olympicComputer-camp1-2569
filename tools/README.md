# 🧰 tools/ — คู่มือสคริปต์

สรุปว่าแต่ละตัวทำอะไร ใช้ตอนไหน — เปิดไฟล์นี้เวลาลืม

**โจทย์โหลดมือจาก <https://www.cs.science.cmu.ac.th/compo/> — ไม่มีสคริปต์ดึงอัตโนมัติ**

---

## อยากทำอะไร → ใช้ตัวไหน

| อยาก... | รัน |
|---|---|
| จัด PDF ที่โหลดมือมาแบบลอย ๆ เข้าโฟลเดอร์ | `fish tools/organize.fish <โฟลเดอร์> --cpp` |
| ได้คะแนนแล้ว อัปเดตตาราง + ย้ายข้อที่เต็ม | `python3 tools/sync_done.py` |
| เช็คว่าข้อไหนยังไม่ได้โหลด (ไม่อัปเดตอะไร) | `python3 tools/find_missed.py` |
| เติมชื่อโจทย์ในชื่อโฟลเดอร์ (ปกติ `organize` ทำให้อยู่แล้ว) | `python3 tools/folder_names.py --apply` |

มีแค่นี้ — 4 ตัว

---

## แต่ละตัวทำอะไร

### `organize.fish` — จัด PDF ที่โหลดมือ

ไฟล์ที่โหลดมาชื่อมักรก เช่น `โจทย์ สอวน (th).pdf` → สร้างโฟลเดอร์ `โจทย์_สอวน/`
แล้วย้ายเป็น `โจทย์_สอวน.pdf` (ตัดวงเล็บ แทนเว้นวรรคด้วย `_` เก็บชื่อไทยครบ)

```fish
fish tools/organize.fish ~/Downloads --dry-run      # ดูเฉย ๆ ยังไม่ย้าย
fish tools/organize.fish ~/Downloads --cpp          # ย้าย + สร้าง .cpp จาก template.cpp
fish tools/organize.fish docs/sheets --prefix 20261005_AM
```

ถ้าโหลด Statement มาแล้วชื่อเป็นรหัสโจทย์อยู่แล้ว (`C1PC01.pdf`) จะได้โฟลเดอร์
`01_C1PC01_Alice_and_Bob/` พร้อม `C1PC01.pdf` + `C1PC01.cpp` ข้างใน — โฟลเดอร์ได้
ลำดับ+ชื่อโจทย์จาก `tools/scores/*.txt` (ผ่าน `folder_names.py`) ส่วนไฟล์ข้างในคง
ชื่อรหัสเปล่า ๆ ตามที่ grader บังคับตอน submit

รหัสที่ยังไม่มีในไฟล์คะแนนจะได้โฟลเดอร์ชื่อรหัสตามปกติ (`C1PC99/C1PC99.pdf`)

**ทำทีละโฟลเดอร์** — จัดผิดที่ก็ย้ายกลับได้ ไฟล์ไม่หาย

### `sync_done.py` — ตัวหลักหลังได้คะแนน

อ่าน `tools/scores/*.txt` → ข้อที่ได้เต็มย้ายเข้า `_done/` → สร้างตารางคะแนนใน
`README.md` ใหม่ทั้งบล็อก → บอกข้อที่ยังไม่มีโฟลเดอร์

`tools/scores/*.txt` คือ**ความจริง** — ตารางใน README ห้ามแก้มือ
รันซ้ำได้เรื่อย ๆ ไม่พัง (ย้ายไปแล้วจะข้าม)

### `find_missed.py` — เช็คอย่างเดียว

เทียบ `tools/scores/*.txt` กับโฟลเดอร์จริงในเครื่อง บอกว่าข้อไหนยังขาด
**ไม่แก้อะไรเลย** ต่างจาก `sync_done.py`

### `folder_names.py` — เติมชื่อโจทย์ในชื่อโฟลเดอร์

เปลี่ยน `C1PC01/` → `01_C1PC01_Alice_and_Bob/` โดยดึงชื่อโจทย์และลำดับจาก
`tools/scores/*.txt` ผ่าน `scorelib` **ไฟล์ข้างในไม่ถูกแตะ**

```fish
python3 tools/folder_names.py             # ดูเฉย ๆ ว่าอะไรจะถูกเปลี่ยน
python3 tools/folder_names.py --apply     # เปลี่ยนจริง (ใช้ git mv เก็บประวัติ)
python3 tools/folder_names.py --print-map # พิมพ์ "รหัส<TAB>ชื่อโฟลเดอร์" ให้ organize.fish ใช้
```

ปกติ **ไม่ต้องรันเอง** — `organize.fish` ตั้งชื่อโฟลเดอร์ให้ตั้งแต่ตอนจัดอยู่แล้ว
ตัวนี้มีไว้สำหรับโฟลเดอร์เก่าที่สร้างไว้ก่อนมีชื่อโจทย์ (รันซ้ำได้ ไม่พัง)

### `scorelib.py` — โมดูลกลาง

ไม่ได้รันเอง ไม่มี CLI — สามตัวข้างบนเรียกใช้ร่วมกัน มีหน้าที่อ่านทะเบียน contest,
อ่านไฟล์คะแนน, แปลงชื่อโฟลเดอร์ (`folder_name_for` / `task_id_from_folder`),
และหารหัสโจทย์เจอโดยเดินทั่ว `problems/`

---

## ไฟล์ข้อมูล (ไม่ใช่สคริปต์)

| ไฟล์ | คืออะไร |
|---|---|
| `graders.txt` | ทะเบียน contest — ชื่อไฟล์คะแนน + ชื่อที่โชว์ใน README |
| `scores/<ชื่อ>.txt` | ตารางคะแนนก็อปจากหน้าเว็บ |

ทั้งสองอย่างเป็น plain text ขึ้น git ได้ ไม่มีความลับ

---

## contest มีหลายตัว — แยกกันยังไง

เว็บ <https://www.cs.science.cmu.ac.th/compo/> มีหลาย contest แต่ละอันเป็น CMS
คนละตัว คนละชุดรหัสโจทย์ ทะเบียนอยู่ที่ `graders.txt`

| ชื่อในสคริปต์ | ปลายทาง |
|---|---|
| `precamp` | `problems/1_precamp/cmu-grader/` |
| `camp1_1` | `problems/2_incamp/1_first_half/cmu-grader/` |
| `camp1_2` | `problems/2_incamp/2_second_half/cmu-grader/` |
| `pretest` | `problems/2_incamp/pretest/` |
| `camp1_ex1` | `problems/2_incamp/exam/ex1/` |
| `camp1_ex2` | `problems/2_incamp/exam/ex2/` |

**เพิ่ม contest ใหม่** = เพิ่ม 1 บรรทัดใน `graders.txt` + สร้างไฟล์ใน `scores/`
ไม่ต้องแก้สคริปต์

`sync_done.py` ไม่ต้องรู้ว่าข้อไหนอยู่ contest ไหน — มันหารหัสโจทย์เจอเองจาก
ชื่อโฟลเดอร์ (ถอด `01_C1PC01_Alice_and_Bob` → `C1PC01` ด้วย `task_id_from_folder`)
รหัสโจทย์ไม่ซ้ำกันอยู่แล้ว

---

## ลำดับใช้งานปกติ

```
1. เปิดเว็บ grader → โหลด Statement ที่ยังไม่มี (ดูรายการจาก find_missed.py)
2. fish tools/organize.fish <โฟลเดอร์ที่โหลดมา> --cpp
3. เขียนโค้ดที่ problems/.../13_C1PR01_Test/C1PR01.cpp   (Ctrl+Shift+B = build + run)
4. ก็อปตารางคะแนนจากหน้าเว็บ → tools/scores/precamp.txt
5. python3 tools/sync_done.py
```

---

## เจอปัญหา

| อาการ | สาเหตุ / ทางแก้ |
|---|---|
| `organize` บอก "มีโฟลเดอร์อยู่แล้ว" | โฟลเดอร์โจทย์ข้อนั้นมีอยู่ — ย้าย PDF เข้าไปเองได้เลย |
| `sync_done` บอก "ยังไม่มีไฟล์คะแนน" | ต้องวางที่ `tools/scores/<ชื่อใน graders.txt>.txt` |
| `sync_done` บอก "อ่านได้ 0 บรรทัด" | ตารางที่ก็อปมาต้องมีคอลัมน์คะแนน `0 / 100` และคั่นด้วย TAB |
| ชื่อโจทย์ในตารางเป็นช่องว่าง | ปกติ — grader บางหน้าตัดชื่อโจทย์ออก คะแนนยังถูก |
| ตารางใน README ไม่ตรง | อย่าแก้ README — แก้ที่ `tools/scores/*.txt` แล้วรันใหม่ |

---

## ฉบับเปิดในเบราว์เซอร์

เนื้อหาเดียวกับไฟล์นี้ แต่จัดหน้าให้อ่านง่ายกว่า — เปิด `tools-report.html`
ที่ราก repo (มีตารางเปรียบเทียบ contest กับแผนผังโฟลเดอร์ด้วย)
