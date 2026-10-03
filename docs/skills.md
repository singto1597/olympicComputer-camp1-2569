# 💻 สอวน. คอม ค่าย 1 (2569) — Lessons จากเวิร์กสเปซโจทย์และเครื่องมือ

> ส่วนนี้บันทึกบทเรียนที่เจอจริงระหว่างทำเวิร์กสเปซค่าย 1 สอวน. คอมพิวเตอร์ 2569
> ตั้งแต่โครงสร้างโฟลเดอร์ → กติกาการตั้งชื่อ → สคริปต์ใน `tools/` → ขั้นตอนทำงาน → ข้อจำกัดของเครื่อง

### 🛠️ ข้อมูลจริงอยู่บน `/mnt/storage` ไม่ใช่ `/home` — เข้าได้สองทางผ่าน symlink
- **Context/Problem:** repo อยู่ที่ `/mnt/storage/03_Academics/OCom_2026/Camp1/olympicComputer-camp1-2569` แต่เปิดจาก IDE ได้ทาง `/home/singto1597/Academics/OCom_2026/Camp1/olympicComputer-camp1-2569` — path เดียวกันโผล่ได้ 2 หน้าตาใน log, สคริปต์ และข้อความ error
- **Root Cause:** `~/Academics` เป็น symlink ชี้ไป `/mnt/storage/03_Academics` (ดิสก์ลูกที่เก็บข้อมูลจริง 885 GB) — ไม่ใช่ที่เก็บจริง
- **Correct Pattern/Solution:** เวลาอ้าง path ในสคริปต์/เอกสารให้ใช้ฝั่ง `/mnt/storage` เป็นหลัก · `tools/organize.fish` แก้ปัญหานี้โดยตั้ง `$root` จาก `realpath` ของตัวสคริปต์เอง → รันจากโฟลเดอร์ไหนก็ได้ และ path ที่ไม่ขึ้นต้น `/` จะถูกต่อกับ `$root` ให้อัตโนมัติ
- **Date Added:** 2026-09-28

### 🛠️ โฟลเดอร์โจทย์ = `<ลำดับ>_<รหัส>_<ชื่อโจทย์>` แต่ไฟล์ข้างในเป็นรหัสเปล่า ๆ
- **Context/Problem:** เดิมโฟลเดอร์ชื่อรหัสเปล่า ๆ (`C1PC01/`) เปิด `cmu-grader/` แล้วต้องนั่งเปิด PDF ทีละอันกว่าจะรู้ว่าข้อไหนคือข้ออะไร — และโฟลเดอร์เรียงตามรหัส (`C1PC` → `C1PE` → `C1PR`) ไม่ตรงกับลำดับโจทย์บนเว็บ (`C1PC` → `C1PR` → `C1PE`)
- **Root Cause:** ชื่อโจทย์มีอยู่ในคอลัมน์ Name ของตารางคะแนนอยู่แล้ว (`tools/scores/*.txt`) แต่ไม่ได้ถูกใช้เป็นชื่อโฟลเดอร์ · ส่วน**ไฟล์ต้องคงชื่อรหัสเปล่า ๆ** เพราะ grader บังคับชื่อไฟล์ตอน submit (คอลัมน์ Files = `C1PC01[.cpp|.c]`) ใส่ชื่อโจทย์ไปต้อง rename กลับทุกครั้ง
- **Correct Pattern/Solution:** `<grader-dir>/01_C1PC01_Alice_and_Bob/C1PC01.cpp` + `C1PC01.pdf` — เลขลำดับมาจากบรรทัดใน `tools/scores/<contest>.txt` (ลำดับเดียวกับตารางใน README) นับ 1 ใหม่ทุก contest · โฟลเดอร์กับไฟล์ชื่อไม่เหมือนกัน**โดยตั้งใจ** · แปลงชื่อด้วย `folder_names.py` / `scorelib.folder_name_for()` เท่านั้น อย่า rename มือ · statement HTML เก็บเป็น `C1PR01.html` · เทสเคสซ้อนใน `testcases/1.in`, `1.out` · เลขนำหน้ามีแค่ระดับ phase (`1_precamp` → `2_incamp` → `3_endcamp`) เพราะต้องเรียงตามเวลา
- **Date Added:** 2026-10-03 (แก้ทับ entry เดิม 2026-09-28 ที่บอกว่า "ไม่ใส่เลขนำหน้า")

### 🛠️ `_done/` ให้สคริปต์ย้าย ไม่ย้ายมือ — และต้องมี `.gitkeep` ค้างไว้
- **Context/Problem:** ข้อที่ได้ 100/100 ต้องย้ายเข้า `_done/` ของ grader นั้น แต่พอ clone repo ใหม่โฟลเดอร์ `_done/` หายไป
- **Root Cause:** git ไม่ track โฟลเดอร์เปล่า
- **Correct Pattern/Solution:** อย่าย้ายมือ — ปล่อยให้ `python3 tools/sync_done.py` ย้าย (ใช้ `git mv` ถ้าไฟล์ถูก track แล้ว ไม่งั้น fallback เป็น `shutil.move`) · โฟลเดอร์ `_done/` ทุกอันมี `.gitkeep` ค้างไว้ (commit `c4fbc5d`) · ถ้าปลายทางมีอยู่แล้วสคริปต์จะข้ามและเตือน ไม่ทับ
- **Date Added:** 2026-09-28

### 🛠️ ตารางคะแนนใน `README.md` สร้างอัตโนมัติ — ห้ามแก้มือ
- **Context/Problem:** อยากแก้ตัวเลขคะแนน/ชื่อโจทย์ในตาราง README ตรง ๆ
- **Root Cause:** ตารางอยู่ในบล็อก `<!-- SCORE:START -->` / `<!-- SCORE:END -->` ที่ `sync_done.py` เขียนทับใหม่ทั้งบล็อกทุกครั้งที่รัน
- **Correct Pattern/Solution:** ของจริงคือ `tools/scores/<ชื่อ contest>.txt` — แก้ที่ไฟล์นั้นแล้วรัน `python3 tools/sync_done.py` · ถ้าหา marker ไม่เจอ สคริปต์จะข้ามและพิมพ์เตือน ไม่ไปทำ README พัง
- **Date Added:** 2026-09-28

### 🛠️ ชื่อไฟล์จาก grader มี suffix ภาษาไทยติดมา `(th)` `(TH)` `(th2)` `(th3)` `(th0)`
- **Context/Problem:** 36 ไฟล์พรีแคมป์ชื่อ `C1PC01 (th).pdf`, `C1PC08 (th2).pdf`, `C1PR02 (TH).pdf`, `C1PR01 (th3).pdf`, `C1PE01 (th0).pdf` — หน้าตาไม่เหมือนกันเลย ถ้าวางโฟลเดอร์ตามชื่อไฟล์ตรง ๆ จะได้โฟลเดอร์ชื่อมีวงเล็บ
- **Root Cause:** เว็บให้โหลดหลายฉบับ (ไทย/อังกฤษ/เวอร์ชันแก้) แล้วต่อ suffix ท้ายชื่อไฟล์ — **ไม่ใช่โจทย์คนละข้อ** รหัสโจทย์คือส่วนหน้าเท่านั้น
- **Correct Pattern/Solution:** `organize.fish` ตัดด้วย `string replace -ra '\s*\([^)]*\)' ''` (ตัดเฉพาะวงเล็บ + ช่องว่างที่นำหน้า) แล้วแทนเว้นวรรคด้วย `_` → รหัสที่เหลือ (`C1PC01`) เอาไปหาชื่อโฟลเดอร์จาก `tools/scores/*.txt` ต่อ → `01_C1PC01_Alice_and_Bob/C1PC01.pdf` · ข้อจำกัดที่รู้: regex ตัดวงเล็บ**ทุกชนิด** ถ้าอนาคตมีชื่อโจทย์ที่มีวงเล็บเป็นส่วนของชื่อจริงจะโดนตัดไปด้วย
- **Date Added:** 2026-10-03

### 🛠️ `organize.fish` มี `--dry-run` — ดูก่อนย้ายจริงเสมอ
- **Context/Problem:** ต้องย้าย PDF 36 ไฟล์รวดเดียว พลาดแล้วต้องมานั่งไล่ย้อน
- **Root Cause:** งานที่แตะไฟล์เยอะควรเห็นรายการปลายทางก่อนลงมือ
- **Correct Pattern/Solution:** `fish tools/organize.fish <โฟลเดอร์> --dry-run` จะพิมพ์ `ไฟล์ → โฟลเดอร์/ไฟล์` ให้ดูก่อน แล้วค่อยรันจริง · ถ้ามีโฟลเดอร์ปลายทางอยู่แล้วจะ **ข้าม** และทิ้ง PDF ไว้ข้างนอก (ไม่ทับ ไม่ย้าย) · สคริปต์ทำทีละโฟลเดอร์ จึงย้อนง่ายถ้าจัดผิดที่
- **Date Added:** 2026-10-03

### 🛠️ `--cpp` ก็อป `template.cpp` ให้ทุกข้อ — แต่ก็อปครั้งเดียวตอนสร้างโฟลเดอร์
- **Context/Problem:** ทุกข้อต้องเริ่มจากโครงเดียวกัน ไม่งั้นต้องมานั่งสร้างไฟล์เปล่าเอง 36 ครั้ง
- **Root Cause:** ลดงานซ้ำตอนเปิดโจทย์ใหม่ สคริปต์จึงก็อปให้ทันทีที่สร้างโฟลเดอร์ — ไม่ได้ผูกกับ template อีกหลังจากนั้น
- **Correct Pattern/Solution:** `fish tools/organize.fish <โฟลเดอร์> --cpp` — สร้าง `<ID>.cpp` จาก `template.cpp` ให้ทุกข้อ · **ไม่ทับ** `.cpp` ที่มีอยู่แล้ว (ตั้งใจ — กันโค้ดที่เขียนไว้หาย) · ถ้าไม่เจอ `template.cpp` จะสร้างไฟล์เปล่าแทน · `template.cpp` เป็นของผู้ใช้ ปรับได้ตามใจ — พอปรับแล้วของเก่าจะไม่ตามไปด้วย ดู entry เรื่อง template drift
- **Date Added:** 2026-10-03

### 🛠️ format บรรทัดคะแนนที่ `scorelib.py` อ่านออก
- **Context/Problem:** วางตารางคะแนนจากเว็บลง `tools/scores/*.txt` แล้ว `sync_done.py` บอก "อ่านได้ 0 บรรทัด"
- **Root Cause:** ตัวจับบรรทัดคือ `_LINE_RE = ^\s*(\d+)\s*/\s*(\d+)\s+([A-Za-z0-9_][A-Za-z0-9_.\-]*)\s*(.*)$` — ต้องขึ้นต้นด้วย `คะแนน / เต็ม` แล้วตามด้วยรหัสโจทย์ ถ้าไม่มีคอลัมน์คะแนนบรรทัดนั้นถูกข้ามทันที
- **Correct Pattern/Solution:** ก็อปตารางจากเว็บมาวางทั้งหัวตารางได้ (บรรทัดที่จับไม่ได้จะถูกข้ามเอง) · ฟิลด์หลังรหัสโจทย์แยกด้วย TAB (`\t+`) **หรือ** ช่องว่าง 2 ตัวขึ้นไป (`_FIELD_SPLIT_RE`) · task_id ซ้ำจะถูก dedup เก็บอันแรก
- **Date Added:** 2026-09-28

### 🛠️ ชื่อโจทย์มีช่องว่างเดี่ยว — ต้องคั่นฟิลด์ด้วย TAB ไม่ใช่ช่องว่าง
- **Context/Problem:** ตอนสร้าง `tools/scores/precamp.txt` ด้วยสคริปต์ ถ้าใช้ช่องว่างเดี่ยวคั่นฟิลด์ ชื่อโจทย์จะกลืนกับ time limit
- **Root Cause:** `_FIELD_SPLIT_RE` แยกที่ TAB หรือช่องว่าง **2 ตัวขึ้นไป** — ชื่ออย่าง `Box Box Box`, `Banana Mega-Trading`, `C1PE01 - Constructor` มีช่องว่างเดี่ยวเต็มไปหมด
- **Correct Pattern/Solution:** คั่นด้วย `\t` ตามที่เว็บ grader ให้มา (`"\t".join([...])`) · ชื่อโจทย์ที่มีช่องว่างเดี่ยวปลอดภัย เพราะไม่ถึงเกณฑ์ 2 ตัว · เขียนไฟล์ด้วย `encoding="utf-8"` เสมอ
- **Date Added:** 2026-10-03

### 🛠️ ลำดับบรรทัดใน `tools/scores/*.txt` = ลำดับที่โชว์ใน README
- **Context/Problem:** หน้าพรีแคมป์บนเว็บเรียงโจทย์แปลก ๆ — `C1PC01`–`C1PC12` → `C1PR01`–`C1PR18` → `C1PE01`–`C1PE04` → แล้วค่อย `C1PC13`, `C1PC14` มาท้ายสุด
- **Root Cause:** `parse_score()` คงลำดับในไฟล์ตามเดิม, `render_group()` วนตามนั้น — **ไม่มีการ sort** และเว็บก็ไม่ได้เรียงตามรหัส
- **Correct Pattern/Solution:** อย่าไป sort ไฟล์คะแนนถ้าอยากได้ลำดับเหมือนเว็บ · โฟลเดอร์ในเครื่องเรียงตามชื่อเองอยู่แล้ว (`C1PC*` → `C1PE*` → `C1PR*`) → **ลำดับใน README กับการเรียงโฟลเดอร์ในเครื่องจะไม่ตรงกันเป็นเรื่องปกติ** ไม่ใช่บั๊ก
- **Date Added:** 2026-10-03

### 🛠️ `find_missed.py` อ่านอย่างเดียว — `sync_done.py` เป็นตัวแก้ของ
- **Context/Problem:** ต้องรู้ว่าข้อไหนยังไม่ได้โหลด โดยไม่อยากให้มีอะไรถูกย้าย/เขียนทับระหว่างเช็ค
- **Root Cause:** สองงานนี้ต้องแยกกัน — เช็คเฉย ๆ กับลงมือแก้
- **Correct Pattern/Solution:** `python3 tools/find_missed.py` = รายงานอย่างเดียว ปลอดภัยที่จะรันบ่อย ๆ · `python3 tools/sync_done.py` = ย้ายข้อที่ได้เต็มเข้า `_done/` + เขียนตาราง README ใหม่ + บอกข้อที่ยังไม่มีโฟลเดอร์ · ทั้งคู่แชร์โค้ดอ่านไฟล์กันผ่าน `tools/scorelib.py` (ไม่มี CLI รันเอง)
- **Date Added:** 2026-09-28

### 🛠️ เพิ่ม contest ใหม่ = เพิ่ม 1 บรรทัดใน `tools/graders.txt` + 1 ไฟล์ใน `scores/`
- **Context/Problem:** เว็บ <https://www.cs.science.cmu.ac.th/compo/> มีหลาย contest แต่ละอันเป็น CMS คนละตัว คนละชุดรหัสโจทย์
- **Root Cause:** อยากให้เพิ่ม contest ได้โดยไม่ต้องแก้โค้ด Python/fish เลย
- **Correct Pattern/Solution:** `graders.txt` ใช้ `<ชื่อไฟล์ใน scores/>` + TAB + `<ชื่อที่โชว์>` · `#` = คอมเมนต์ · **ลำดับในไฟล์ = ลำดับหัวข้อใน README** · แล้ววางตารางคะแนนที่ `tools/scores/<ชื่อนั้น>.txt` · `sync_done.py` หาโฟลเดอร์โจทย์เจอเองจากรหัส จึงไม่ต้องมีตารางว่า contest ไหนอยู่โฟลเดอร์ไหน
- **Date Added:** 2026-09-28

### 🛠️ ห้ามเสนอสคริปต์ล็อกอิน/สแครป grader — โจทย์โหลดมือเท่านั้น
- **Context/Problem:** อยากได้โจทย์ครบเร็ว ๆ ก็คิดว่าจะเขียนสคริปต์ดึงอัตโนมัติ
- **Root Cause:** ผู้ใช้เลือกโหลดเองจากหน้าเว็บ และ repo นี้ไม่เก็บรหัส/cookie อะไรเลย (`.gitignore` กัน `tools/secrets.fish` กับ `tools/.cookies.*.txt` ไว้แล้ว) — commit `a51b7a4` "เลิกดึงโจทย์อัตโนมัติ" คือบทเรียนนี้
- **Correct Pattern/Solution:** สคริปต์ที่มีอยู่ช่วยได้แค่งานหลังโหลด — `find_missed.py` บอกว่าข้อไหนขาด, `organize.fish` จัดไฟล์ที่โหลดมาแล้วเข้าโฟลเดอร์ · อย่าเสนอ login/scrape ซ้ำอีก
- **Date Added:** 2026-09-28

### 🛠️ ปุ่ม build/run ใน VSCode — 3 task ใน `.vscode/tasks.json`
- **Context/Problem:** ต้องมีรอบคอมไพล์/รันที่ตรงกับลิมิตของโจทย์ และไม่ต้องพิมพ์คำสั่งยาวทุกครั้ง
- **Root Cause:** โจทย์ competitive programming อ่านจาก stdin เขียนไป stdout ทุกข้อ
- **Correct Pattern/Solution:**
  - `Ctrl+Shift+B` → `build + run (C++)`: `g++ -std=c++17 -O2 -Wall -Wextra` อ่าน input จาก `${workspaceFolder}/.run/input.in` เขียนผลลง `.run/output.txt` แล้ว `cat` ให้ดูพร้อม exit code
  - `run with testcases/ (C++)`: วน `testcases/*.in` เทียบกับ `.out` ด้วย `diff -b` (ไม่สนช่องว่าง) แล้วสรุป `N / M passed`
  - `build only (C++)`: คอมไพล์อย่างเดียว
  - `.run/` กับ `*.out` ถูก gitignore และซ่อนใน file explorer (`.vscode/settings.json`)
- **Date Added:** 2026-09-28

### 🛠️ `.gitattributes` กัน PDF พังตอน commit
- **Context/Problem:** repo เก็บ PDF หลักฐานผลงานเป็นสิบไฟล์ กลัว git ไปยุ่งกับ line ending แล้วไฟล์เสีย
- **Root Cause:** `* text=auto eol=lf` ถ้าไม่ประกาศข้อยกเว้น git จะถือว่าไฟล์ไบนารีเป็น text
- **Correct Pattern/Solution:** ประกาศ `*.pdf binary` ไว้ (พร้อม `png/jpg/jpeg/gif/zip/exe/out/o/bmp/docx/pptx`) ใต้ `* text=auto eol=lf` · ไฟล์ `.cpp` ยัง normalize เป็น LF ตามปกติ
- **Date Added:** 2026-09-28

### 🛠️ เครื่องนี้ควบคุม GUI ไม่ได้เลย — งานที่ต้องกดเว็บต้องให้ผู้ใช้ทำเอง
- **Context/Problem:** อยากให้ช่วยกดโหลด statement จากหน้าเว็บ grader แทน
- **Root Cause:** Wayland/GNOME + ไม่มี `xdotool`/`ydotool`/`wtype` และ `/dev/uinput` ไม่ให้ user เขียน → คลิก/พิมพ์แทน/สกรีนช็อต ทำไม่ได้ทั้งหมด · Chrome 150 ก็ใช้ `--remote-debugging-port` กับ profile จริงไม่ได้แล้ว (บล็อกตั้งแต่ 136)
- **Correct Pattern/Solution:** ออกแบบงานให้เป็น CLI ล้วน — ให้ผู้ใช้โหลดไฟล์เองแล้วค่อยสั่งสคริปต์จัด · ข้อความช่วยเหลือต้องบอกขั้นตอนที่คนทำ ไม่ใช่บอกให้ AI ไปกดให้
- **Date Added:** 2026-09-28

### 🛠️ เครื่องนี้ไม่มีตัวอ่าน PDF — แต่อย่าลืมว่า Read tool เปิด PDF ได้
- **Context/Problem:** อยากยืนยันว่าเนื้อใน `C1PC01.pdf` ตรงกับชื่อโจทย์จริง ไม่ใช่แค่เชื่อชื่อไฟล์
- **Root Cause:** เครื่องนี้ไม่มี `pdftotext` (poppler-utils ไม่ได้ลง) และ Python ก็ไม่มี `pypdf`/`PyPDF2`/`fitz` → เช็คด้วย CLI ไม่ได้ · ลงเพิ่มต้อง `sudo` ซึ่งต้องถามก่อน
- **Correct Pattern/Solution:** ถ้าต้องดูเนื้อใน PDF ให้ใช้ **Read tool ของ Claude Code เปิด PDF ตรง ๆ** (รับ `pages` ได้ ไม่ต้องพึ่ง CLI อะไรเลย) · ถ้าต้องการ `pdftotext` ไว้ใช้ในสคริปต์จริง ๆ ค่อยขออนุญาตลง poppler-utils · อย่าประกาศว่า "ตรวจเนื้อในให้แล้ว" ถ้ายังไม่ได้เปิดจริง — ยืนยันได้แค่ชื่อไฟล์/โฟลเดอร์
- **Date Added:** 2026-10-03

### 🛠️ shell เริ่มต้นคือ fish — สคริปต์ใน `tools/` ต้องเป็น syntax fish
- **Context/Problem:** เขียนสคริปต์ bash ลง `tools/` แล้วรันไม่ผ่าน
- **Root Cause:** เครื่องนี้ใช้ `/usr/bin/fish` เป็น shell หลัก — fish ไม่รองรับ heredoc และ syntax ต่างจาก bash (ตัวแปรเป็น list, ไม่มี `$(( ))`, ใช้ `set`, `math`, `string`)
- **Correct Pattern/Solution:** สคริปต์จัดการไฟล์ใช้ fish + `argparse` built-in (`argparse 'cpp' 'dry-run' 'prefix=' 'h/help' -- $argv`) · งานที่ต้อง parse เยอะ ๆ (คะแนน, เอกสาร) ใช้ Python 3 แทน · อย่าเขียน bash แล้วหวังว่าจะรันได้
- **Date Added:** 2026-09-28

### 🛠️ ภาษาไทยในชื่อไฟล์และในข้อมูล — UTF-8 เสมอ + quote ทุกครั้ง
- **Context/Problem:** ไฟล์คะแนนมีชื่อโจทย์/คำอธิบายไทย สไลด์ก็ตั้งชื่อไทยได้ และ shell เป็น fish ที่ตัวแปรไม่ตัดคำแบบ bash
- **Root Cause:** ชื่อไทยมีทั้งในไฟล์คะแนน (`tools/scores/*.txt`) และในชื่อไฟล์/โฟลเดอร์จริง
- **Correct Pattern/Solution:** เปิดไฟล์ด้วย `encoding="utf-8"` ทุกครั้ง (ใน `scorelib.py` ทำแล้ว) · quote path ทุกครั้งใน shell — `"$file"`, `'...'` · เวลาสร้างสคริปต์ที่วนชื่อไฟล์ อย่าใช้ byte-length ตัดสินอะไร
- **Date Added:** 2026-09-28

### 🛡️ `sudo` ต้องถามก่อนทุกครั้ง และ group `docker` = เทียบเท่า root
- **Context/Problem:** คำสั่งอย่างลง package, mount path, แก้ config ระบบ ต้องใช้สิทธิ์สูง
- **Root Cause:** ผู้ใช้อยู่ใน group `docker` → คำสั่ง docker ที่ mount path ได้เท่ากับเขียนไฟล์ระบบได้
- **Correct Pattern/Solution:** ห้ามรัน `sudo` อัตโนมัติ ต้องขออนุญาตก่อนทุกครั้ง · ระวังเป็นพิเศษกับคำสั่ง docker ที่ mount path — โดยเฉพาะใน repo นี้ที่ PDF เป็นหลักฐานผลงาน
- **Date Added:** 2026-09-28

### 🛡️ โฟลเดอร์ความลับอยู่นอก repo นี้ — ห้ามอ่าน ห้าม commit
- **Context/Problem:** ใต้ `/mnt/storage` มีโฟลเดอร์เก็บ SSH keys/VPS creds/API keys, config เครือข่ายพร้อมรหัสผ่านระบบ และเรื่องส่วนตัว/ครอบครัว/การเงิน ปนอยู่กับโฟลเดอร์งานเรียน
- **Root Cause:** ดิสก์ลูกเดียวกันเก็บทั้งงานที่แชร์ได้และความลับ — และ repo นี้อยู่บน GitHub (`singto1597/olympicComputer-camp1-2569`)
- **Correct Pattern/Solution:** ห้าม `cat`/`grep`/ส่งออกเนื้อหาจากโฟลเดอร์เหล่านั้น (ดูชื่อโฟลเดอร์เต็มใน `CLAUDE.md` ระดับเครื่อง) · ดูได้แค่ชื่อไฟล์ถ้าจำเป็น · **ห้าม commit อะไรจากที่นั่นเด็ดขาด** · เอกสารนี้จงใจไม่ระบุ path เต็มของโฟลเดอร์เหล่านั้น เพราะไฟล์นี้ขึ้น GitHub
- **Date Added:** 2026-10-03

### 🛠️ `docs/`, PDF โจทย์ และไฟล์คะแนน = หลักฐานผลงาน ห้ามลบ/ย้ายโดยไม่ถาม
- **Context/Problem:** อยากจัดระเบียบใหม่แล้วย้าย/ลบของเก่า
- **Root Cause:** โฟลเดอร์นี้เป็นแฟ้มสะสมผลงานการเรียน (พอร์ต POSN) — ของเก่าเป็นหลักฐานว่าทำอะไรมาบ้าง
- **Correct Pattern/Solution:** ถามก่อนทุกครั้งที่จะลบ/ย้าย · การจัดเข้าโฟลเดอร์ด้วย `organize.fish` ถือว่าปลอดภัยเพราะไม่ลบไฟล์ (`mv` เท่านั้น) แต่ก็ยังใช้ `--dry-run` ก่อนเมื่อทำเยอะ ๆ
- **Date Added:** 2026-09-28

### 🧯 รอบ 2026-10-03: จัดโจทย์พรีแคมป์ 36 ข้อเข้าครบทั้งชุด
- **Context/Problem:** โหลด PDF พรีแคมป์มา 36 ไฟล์ลอยอยู่ใน `problems/1_precamp/cmu-grader/` ยังไม่มีโฟลเดอร์ และยังไม่มีไฟล์คะแนนให้ `sync_done.py` ใช้
- **Root Cause:** ขั้นตอนปกติต้องผ่าน `organize.fish` + ก็อปตารางคะแนนจากเว็บ ซึ่งยังไม่ได้ทำ
- **Correct Pattern/Solution:** ลำดับที่ใช้ได้ผล —
  1. `fish tools/organize.fish problems/1_precamp/cmu-grader --cpp --dry-run` (ดู 36 รายการก่อน)
  2. รันจริงโดยตัด `--dry-run` ออก → 36 ไฟล์เข้าโฟลเดอร์ + `.cpp` ครบ 36
  3. สร้าง `tools/scores/precamp.txt` (TAB-separated, คงลำดับตามเว็บ)
  4. `python3 tools/sync_done.py` → ตาราง 36 แถวใน README, ย้ายเข้า `_done/` 0 ข้อ (ยังไม่มีข้อไหนได้เต็ม)
  5. `python3 tools/find_missed.py` → `✨ ครบถ้วนทั้งหมด!`
  6. (ตามมา later) `python3 tools/folder_names.py --apply` → โฟลเดอร์ 36 อันเปลี่ยนเป็น `01_C1PC01_Alice_and_Bob` … `36_C1PC14_Judgement` ไฟล์ข้างในชื่อเดิมทุกไฟล์
- **ขยาย (ตัวเลขจริง):** พรีแคมป์ = 3 ชุดรหัส รวม 36 ข้อ — `C1PC01`–`C1PC14` (14) · `C1PE01`–`C1PE04` (4) · `C1PR01`–`C1PR18` (18) · ทุกข้อ 1.000 second / 8.00 MiB ยกเว้น `C1PC05` Banana Mega-Trading = 1.500 seconds · ชื่อโจทย์ชุด `C1PE*` ขึ้นต้นด้วยรหัสซ้ำ (`C1PE01 - Constructor`)
- **Date Added:** 2026-10-03

### 🛠️ `notes/`, `docs/`, `3_endcamp/` และโฟลเดอร์โจทย์เว็บอื่นยังว่าง — มีแต่ `.gitkeep`
- **Context/Problem:** เปิด repo ดูแล้วเข้าใจว่ามีข้อมูลเยอะ ทั้งที่ยังไม่มีอะไร
- **Root Cause:** `.gitkeep` ทำให้โฟลเดอร์เปล่าติดไปกับ clone ได้ → โฟลเดอร์ที่มีแต่ `.gitkeep` = "จองที่ไว้ ยังไม่มีของ"
- **Correct Pattern/Solution:** สถานะ ณ 2026-10-03 มีข้อมูลจริงแค่ `problems/1_precamp/cmu-grader/` (36 ข้อพรีแคมป์) กับ `tools/` · `notes/`, `docs/syllabus|sheets|exams/`, `problems/2_incamp/*`, `problems/3_endcamp/`, `codeforces/`, `programming.in.th/` ยังว่าง
- **Date Added:** 2026-10-03

### 🛠️ `tools-report.html` ที่ราก repo = ฉบับอ่านง่ายของ `tools/README.md`
- **Context/Problem:** อยากอธิบายโครงสร้างเครื่องมือให้อ่านสบายตากว่า markdown ใน terminal
- **Root Cause:** `tools/README.md` ยาวและมีตารางเยอะ
- **Correct Pattern/Solution:** `tools-report.html` (30 KB) เนื้อหาเดียวกับ `tools/README.md` แต่จัดหน้าให้อ่านง่ายกว่า และมีตารางเปรียบเทียบ contest กับแผนผังโฟลเดอร์ · แก้ `tools/README.md` แล้วอย่าลืมอัปเดตตัวนี้ด้วย
- **Date Added:** 2026-10-03

### 🛠️ ลิมิต 1 วินาที / 8 MiB ของพรีแคมป์ → `template.cpp` มี fast I/O ติดมาให้เลย
- **Context/Problem:** โจทย์พรีแคมป์ทุกข้อให้เวลา **1.000 second** และหน่วยความจำ **8.00 MiB** (ยกเว้น `C1PC05` Banana Mega-Trading = 1.500 seconds) — เวลาน้อยพอที่การอ่าน input จะมีผล
- **Root Cause:** `cin`/`cout` โหมด default ต้อง sync กับ stdio ของ C ทุกครั้งที่อ่าน/เขียน ทำให้ช้ากว่า `scanf`/`printf` พอสมควร งานที่ input หลักแสนบรรทัดจึงเสียเวลาไปกับการอ่านเปล่า ๆ
- **Correct Pattern/Solution:** `template.cpp` ใส่ `ios_base::sync_with_stdio(false); cin.tie(NULL);` ใน `main()` ตั้งแต่ 2026-10-03 (ผ่าน `-Wall -Wextra` ไม่มี warning) · **ข้อควรระวัง: หลังจากปิด sync แล้วห้ามผสม `cin` กับ `scanf`/`getchar`/`puts`** ไม่งั้นลำดับ input จะสลับกันแบบเงียบ ๆ · หน่วยความจำ 8 MiB เป็นข้อจำกัดที่ต้องคิดจริง — `int` 2 ล้านตัว = 7.6 MiB ก็เกือบเต็มแล้ว (ลิมิตของทุกข้ออยู่ในคอลัมน์ "ลิมิต" ของตาราง README)
- **Date Added:** 2026-10-03

### 🛠️ `organize --cpp` ก็อป template ครั้งเดียว — พอ template เปลี่ยน ต้อง sync ย้อนหลังเอง
- **Context/Problem:** `template.cpp` เพิ่ม fast I/O แต่ `.cpp` 36 ข้อที่ก็อป template ไปตอนจัดโฟลเดอร์ยังเป็นเวอร์ชันเปล่า (`main()` ว่าง) — เปิดข้อใหม่มาเขียนจะได้โค้ดที่ไม่มี fast I/O ทั้งที่ template มีแล้ว
- **Root Cause:** `organize.fish` ก็อป template ณ ตอนสร้างโฟลเดอร์เท่านั้น และกฎ "ไม่ทับ `.cpp` ที่มีอยู่" (ซึ่งถูกต้อง) ก็ทำให้รัน `--cpp` ซ้ำไม่ช่วยอะไร → template กับของที่ก็อปไปแล้ว **drift ออกจากกันได้**
- **Correct Pattern/Solution:** ก่อนทับไฟล์ที่กระจายอยู่ 36 ที่ ต้องพิสูจน์ก่อนว่า **ยังไม่มีโค้ดที่เขียนเองอยู่ในนั้น** ทำสองชั้น: (1) `git status --porcelain -- "problems/**/*.cpp"` ต้องไม่เห็นไฟล์ถูกแก้ (2) เทียบกับ template เก่าที่อยู่ใน git — `git show HEAD:template.cpp > /tmp/old.cpp` แล้ว `diff` ทีละไฟล์ · ครั้งนี้ได้ 36/36 เหมือนกันทุกไบต์ จึง `cp template.cpp` ทับได้ทั้งชุด แล้ว `git status` ต้องขึ้น 36 ไฟล์ตามคาด · ถ้าจะให้ดีกว่านี้คือเพิ่มโหมด sync ให้ `organize.fish` แต่ยังไม่ได้ทำ
- **Date Added:** 2026-10-03

### 🛠️ fish parse error ทำให้ **ทั้ง command line** ไม่รัน — ผลตรวจที่ขัดกับ git status คือสัญญาณว่าเครื่องมือพัง
- **Context/Problem:** รัน block เดียวที่ขึ้นต้นด้วย `git show HEAD:template.cpp > /tmp/old.cpp` แล้วตามด้วยลูป fish ที่ syntax ผิด → ผลคือลูปที่เช็คต่อรายงานว่า "`.cpp` ต่างจาก template เก่า **36/36 ไฟล์**" ซึ่งขัดกับ `git status` ที่บอกว่าไม่มีไฟล์ไหนถูกแก้เลย
- **Root Cause:** สองชั้นซ้อนกัน — (1) **fish parse ทั้ง command line ก่อน execute** เจอ `end` ที่ผิดที่ก็ไม่รันอะไรเลย รวมถึงคำสั่ง `git show` ที่อยู่ต้นบรรทัด → ไฟล์ `/tmp/old.cpp` ไม่ถูกสร้าง (2) `diff -q <ไฟล์ที่ไม่มีอยู่> <ไฟล์จริง>` ล้มเหลวทุกครั้ง และลูปตีความ "ล้มเหลว" เป็น "ต่าง"
- **Correct Pattern/Solution:** ถ้าผลตรวจ "ผิดทุกตัวพร้อมกัน" ให้ **สงสัยเครื่องมือก่อนข้อมูล** — โดยเฉพาะเมื่อมันขัดกับแหล่งความจริงอื่น (`git status`) · เช็คว่าไฟล์ที่ใช้เทียบมีอยู่จริงก่อน (`ls -la /tmp/old.cpp`) อย่าเพิ่งรายงานผล · ลูปที่ซับซ้อนให้ย้ายไป `bash -c '...'` หรือเขียนเป็นไฟล์สคริปต์ อย่ายัดใน command line เดียวกับคำสั่งตั้งต้นที่ผลของมันจำเป็นต่อลูป · ข้อนี้สำคัญเพราะผลที่ได้ดู "น่าเชื่อ" (มีตัวเลข 36/36 ชัดเจน) ทั้งที่ความจริงคือ 0/36
- **Date Added:** 2026-10-03

### 🛠️ ชื่อโฟลเดอร์คือ "คีย์" ของสคริปต์ทั้งชุด — เปลี่ยนชื่อโฟลเดอร์ไม่ใช่แค่ rename
- **Context/Problem:** อยากให้โฟลเดอร์มีชื่อโจทย์ (`C1PC01` → `01_C1PC01_Alice_and_Bob`) คิดว่าเป็นแค่ `mv` ครั้งเดียว แต่ปรากฏว่าสคริปต์ทุกตัวหาข้อมูลด้วย **ชื่อโฟลเดอร์** ไม่ใช่ด้วยรหัสที่เก็บที่อื่น
- **Root Cause:** `scorelib._task_index()` เดิมใช้ `os.path.basename` เป็นคีย์ตรง ๆ → `find_task("C1PC01")` จะไม่เจอ `01_C1PC01_...` · `sync_done.move_into_done()` ตั้งปลายทางเป็น `done_dir_for(...)/task_id` (รหัสเปล่า) → ย้ายเข้า `_done/` แล้ว**ชื่อโจทย์หายเงียบ ๆ** กลับเป็น `C1PC01` · `find_missed.py` พึ่ง `find_task()` → จะรายงานว่า "ขาดทั้ง 36 ข้อ" ทั้งที่มีอยู่ครบ · `organize.fish` สร้างโฟลเดอร์จากชื่อไฟล์ PDF อย่างเดียว จึงไม่รู้ชื่อโจทย์เลย
- **Correct Pattern/Solution:** เพิ่ม `scorelib.folder_name_for(task_id, name, order)` + `scorelib.task_id_from_folder(folder)` เป็น**แหล่งความจริงเดียว** ของกฎการตั้งชื่อ แล้วแก้ทุกจุดให้ใช้: `_task_index()` เก็บคีย์ด้วย `task_id_from_folder(name)` · `move_into_done()` ใช้ `os.path.basename(src)` เป็นปลายทาง (ยกชื่อทั้งดุ้นไป `_done/`) · `folder_names.py` เป็นตัว rename (ใช้ `git mv` → git เก็บเป็น `R` rename ไม่ใช่ `D`+`??`) · `organize.fish` เรียก `--print-map` ของตัวนั้น · **ไฟล์ข้างในไม่แตะเลย** เพราะ grader บังคับชื่อไฟล์ตอน submit
- **บทเรียนทั่วไป:** ก่อน rename สิ่งที่ถูกใช้เป็น "คีย์" ให้ grep หาทุกที่ที่อ่านค่านั้นก่อน — ที่นี่ใช้ subagent เดินหาผู้บริโภคชื่อโฟลเดอร์ทั้ง repo แล้วเจอ 4 สคริปต์ + เอกสาร 5 ไฟล์ที่ต้องแก้ตาม
- **Date Added:** 2026-10-03

### 🛠️ fish ไม่มี associative array — `$m[คีย์]` ใช้ไม่ได้ ต้องใช้ list คู่กัน
- **Context/Problem:** เขียน `organize.fish` ให้สร้าง map `รหัสโจทย์ → ชื่อโฟลเดอร์` ด้วย `set task_names[$parts[1]] $parts[2]` แล้วอ่านด้วย `set -q task_names[$clean]` — ได้ error `set: Invalid index starting at "C1PC01]"` ทุกครั้ง ผลคือไฟล์ทดสอบ **ตกลงไปใช้ fallback ชื่อรหัสเปล่าเงียบ ๆ** (`C1PC01/C1PC01.pdf`) แทนที่จะเป็น `01_C1PC01_Alice_and_Bob/` — ดูเหมือนสคริปต์ทำงานได้ แต่ได้ผลผิด
- **Root Cause:** fish ไม่มี associative array — subscript ของ `set`/ตัวแปร รับ**แต่ตัวเลข** (index ของ list หรือ slice `1..3`) ใส่สตริงเป็นคีย์ไม่ได้ · และเพราะ error นี้ไม่ได้ทำให้สคริปต์หยุด (แค่ตัวแปรนั้นว่าง) ความผิดพลาดจึงเงียบ
- **Correct Pattern/Solution:** ใช้ **list สองอันคู่กัน** แล้วหาตำแหน่งด้วย `contains -i` (คืน index 1-based ถ้าเจอ, ว่างถ้าไม่เจอ):
  ```fish
  set -l task_ids; set -l task_folders
  for line in (python3 tools/folder_names.py --print-map)
      set -l parts (string split \t -- $line)
      set -a task_ids $parts[1]; set -a task_folders $parts[2]
  end
  # ตอนใช้
  set -l idx (contains -i -- $clean $task_ids)
  if test -n "$idx"; set folder $task_folders[$idx]; end
  ```
  · **ทดสอบ branch fallback เสมอ** — เคสที่ "หาไม่เจอ" คือเคสที่บั๊กซ่อนอยู่ (รันกับรหัสปลอม `C1PC99` ด้วย) · ถ้าจำนวน lookup น้อย จะ shell ออกไปถาม Python ทีละครั้งก็ได้ แลกความง่ายกับ process spawn
- **Date Added:** 2026-10-03

### 🛠️ รหัสโจทย์ที่มี `_` ในตัว ถอดด้วย `split("_")[1]` ไม่ได้ — และการเทียบ prefix ต้องบังคับขอบ `_`
- **Context/Problem:** ยกโจทย์ค่าย 1 ครึ่งแรกเข้ามาแล้ว โฟลเดอร์ `01_C1C0_ADD/` กลายเป็นโฟลเดอร์ที่สคริปต์ "มองไม่เห็น" — `find_missed.py` รายงานว่าข้อนี้ยังขาดทั้งที่มีอยู่ และข้อนี้ไม่ถูกย้ายเข้า `_done/` ทั้งที่ได้ 100/100 (รหัสข้ออื่นปกติทุกข้อ)
- **Root Cause:** `task_id_from_folder()` เดิมถอดรหัสด้วย `folder_name.split("_")[1]` ซึ่งสมมติว่ารหัสโจทย์ไม่มี `_` — `01_C1C0_ADD` จึงออกมาเป็น `C1C0` (รหัสที่ไม่มีอยู่จริง) แล้ว `find_task()` หาไม่เจอ → `sync_done` ไม่ย้ายเข้า `_done/` เพราะเทียบ `task_id` ไม่ตรง · อาการเดียวกันนี้โผล่ฝั่ง `organize.fish` ด้วยคนละสาเหตุ: มันเช็ค "มีโฟลเดอร์อยู่แล้ว" ด้วย `string match -- "$clean"'_*' $cand_full` โดยไม่มีขอบเขต ทำให้ `C1T1` ไป match `C1T10_ธงแห่งโลกใหม` → บอกว่ามีโฟลเดอร์อยู่แล้วทั้งที่ยังไม่มี (และกลับกันก็พลาดได้)
- **Correct Pattern/Solution:** อย่าเดาโครงสร้างรหัส — **เทียบกับรายการรหัสที่รู้จักจากไฟล์คะแนน** (`tools/scores/*.txt` ผ่าน `parse_all_scores()` แคชไว้ใน `known_task_ids()`) หาแบบ longest match:
  ```python
  stripped = re.sub(r"^\d+_", "", folder_name)          # ตัดเลขลำดับก่อน
  best = max((t for t in pool
              if stripped == t or stripped.startswith(t + "_")),
             key=len, default="")
  ```
  เงื่อนไข `stripped == t or stripped.startswith(t + "_")` คือหัวใจ — **บังคับขอบ `_`** ไม่งั้น `C1T1` จะ match `C1T10_...` และไม่เลือก `max(key=len)` ก็จะได้รหัสสั้นที่ผิด · ฝั่ง fish แก้ด้วยการตัดเลขลำดับออกแล้วเทียบชื่อเต็ม หรือ `string match -q -- "$clean"'_*' $cand_full` (ขอบ `_` ชัดเจน) · ทดสอบแล้ว: `01_C1C0_ADD`→`C1C0_ADD` · `10_C1T10_ธงแห่งโลกใหม`→`C1T10` · `21_C1T7_เทค่าย`→`C1T7` · `codeforces`→`codeforces` (โฟลเดอร์แปลกปลอมยังตกไป fallback เดิม) และ `C1T1` vs `C1T10` ไม่ชนกัน
- **บทเรียนทั่วไป:** รหัสที่ "หน้าตาเหมือนมีโครงสร้างตายตัว" มักมีข้อยกเว้นเสมอ — ถ้ามีรายการรหัสอยู่แล้ว ให้ **lookup กับรายการนั้น** แทนการ parse เอา และทุกครั้งที่เทียบ prefix ให้ถามตัวเองว่าต้องมี delimiter กั้นไหม
- **Date Added:** 2026-10-03

### 🧯 รอบ 2026-10-03: โจทย์ค่าย 1 ครึ่งแรกเป็นชุดเดิมของปี 2568 — import ทั้งชุด + มาร์ค "100 เก่า"
- **Context/Problem:** เปิดค่าย 1 ครึ่งแรกปี 2569 มาเจอ 54 ข้อ ซึ่งเกือบทั้งหมดเป็น**ชุดเดิมของค่าย 1 ครึ่งแรกปี 2568** (ปี 2568 มี `C1Y8` · `C1Y16` · `C1N12` ที่ปีนี้ไม่มี ส่วนของใหม่จริงมี 3 ข้อ: `C1T40` `C1T41` `C1T42`) — ส่งโค้ดปีก่อนซ้ำได้ 100/100 ทันที **38 ข้อ** ถ้าไม่บันทึกไว้ ตารางคะแนนจะดูเหมือนคืบหน้าปีนี้ทั้งที่ยังไม่ได้เขียนอะไรใหม่เลย
- **Root Cause:** ต้นทางรุ่นเก่าวางโครงสร้างคนละแบบกับ repo นี้ — แบ่งเป็นโฟลเดอร์ย่อยข้อละอัน (`1.CMU_Grader/16.C1T1/{C1T1 (C++).pdf, main.cpp, main.exe, input.in}`) ทำให้ `organize.fish` ใช้ไม่ได้ (มัน glob แค่ `*.pdf` ชั้นเดียว และไม่รู้จัก `main.cpp`) และคะแนน 100 ที่ได้มาไม่ใช่ฝีมือปีนี้
- **Correct Pattern/Solution:** เพิ่ม `tools/import_old_camp.py` (ตัวที่ 5 ของ `tools/`) แล้วใช้ลำดับนี้ —
  1. สร้าง `tools/scores/<contest>.txt` จากตารางบนเว็บก่อน (import เอาชื่อโจทย์+ลำดับไปตั้งชื่อโฟลเดอร์ผ่าน `folder_name_for()` ตัวเดียวกับ `organize.fish`)
  2. `python3 tools/import_old_camp.py <โฟลเดอร์ปีเก่า> --contest camp1_1 --dest problems/2_incamp/1_first_half/cmu-grader --dry-run` → ดูรายการทั้งหมดก่อน แล้วรันจริงโดยตัด `--dry-run`
  3. `python3 tools/folder_names.py` → ต้อง**เงียบ** (ชื่อถูกตั้งแต่ import ถ้ามันเสนอให้เปลี่ยน = ชื่อจากสองทางไม่ตรงกัน)
  4. `python3 tools/find_missed.py` → ที่ยังขาดคือ**โจทย์ใหม่ของปีนี้** ให้โหลด PDF แล้วจัดด้วย `organize.fish` ตามปกติ
  5. `python3 tools/sync_done.py`
  กติกาแปลงไฟล์: `main.cpp`→`<รหัส>.cpp` · `main_optimize.cpp`→`<รหัส>_optimize.cpp` · `<รหัส> (...).pdf`→`<รหัส>.pdf` (ตัดวงเล็บแบบเดียวกับ `organize.fish`) · **ข้าม** `main`/`*.exe`/`input.in`/`output.txt` (repo ใช้ `.run/` + gitignore) · ข้อที่ต้นทางไม่มีโค้ด (8 ข้อ) ได้ `<รหัส>.cpp` จาก `template.cpp` ไปเริ่มเขียน · **default = คัดลอก** ต้นทางไม่ถูกแตะ (`--move` ถ้าต้องการจริง) เพราะ `03_Academics/CLAUDE.md` ห้ามย้ายไฟล์งานเก่าโดยไม่ถาม
- **มาร์ค 100 เก่า 3 ทาง (ทำพร้อมกัน):** (1) คอลัมน์ **หมายเหตุ** ในตาราง README ขึ้น `🕰️ 100 เก่า` — gen จาก `tools/scores/camp1_1.old100_68.txt` (2) ไฟล์ `.old100_68` ในโฟลเดอร์โจทย์ 38 อัน (3) `notes/100เก่า_ค่าย1ครึ่งแรก.md` อธิบายที่มา + ตารางว่าข้อไหนมีโค้ดปีเก่าแต่ยังไม่ผ่าน
- **ข้อควรระวังสำคัญ:** **เขียน marker ในโฟลเดอร์ก่อนรัน `sync_done.py`** เพราะ sync_done ย้าย 38 โฟลเดอร์นั้นเข้า `_done/` ทันที — marker จะติดไปด้วย (ถ้าลืม ก็ยังแปะทีหลังได้ แค่ต้องรัน sync_done ซ้ำ) · และคอลัมน์ `หมายเหตุ` เพิ่ม**เฉพาะ contest ที่มีไฟล์ old100** เท่านั้น ตารางพรีแคมป์จึงหน้าตาเหมือนเดิม ไม่มีคอลัมน์เปล่าโผล่มา
- **ตรวจแล้ว:** `find_missed` ครบ 90 ข้อ (36 พรีแคมป์ + 54 ค่าย 1 ครึ่งแรก) · `folder_names` เงียบ · marker 38 ไฟล์ · `_done/` 38 โฟลเดอร์ เหลือ 16 ข้อที่ยังไม่เต็ม · ต้นทางปี 2568 ยังอยู่ครบ (166 ไฟล์, `main.cpp` 43) · คอมไพล์ตัวอย่าง 7 ข้อด้วย `g++ -std=c++17 -O2` ผ่านหมด
- **Date Added:** 2026-10-03

### 🛠️ ป้ายที่บอก "ที่มา" ของข้อมูล ควรเก็บปีไว้ในชื่อไฟล์ แล้วอ่านแบบ glob
- **Context/Problem:** ตั้งชื่อไฟล์ป้ายว่า `camp1_1.old100.txt` แล้วอ่านชื่อไม่ออกว่า "เก่า" คือปีไหน — ทั้งที่ความหมายจริง ๆ คือ "โค้ดที่ได้ 100 มาจากปี 2568"
- **Root Cause:** ชื่อไฟล์แบบไม่มีปีทำให้ข้อมูลที่สำคัญที่สุด (ที่มา) หายไปจากที่ที่คนเปิดดูเร็วที่สุด · และโค้ดที่ฝังชื่อไฟล์ตายตัว (`f"{name}.old100.txt"`) จะรองรับปีที่สองไม่ได้เลย
- **Correct Pattern/Solution:** ใส่ปี พ.ศ. ย่อของ**โค้ดต้นทาง** ต่อท้ายชื่อไฟล์ (`camp1_1.old100_68.txt`) และให้ตัวอ่าน **glob** แทนชื่อตายตัว:
  ```python
  prefix = f"{self.name}.old100"          # ต้องมีจุดคั่น ไม่งั้น camp1_1 ไปโดน camp1_10
  [n for n in os.listdir(SCORES_DIR)
   if n.startswith(prefix) and n.endswith(".txt")]
  ```
  อ่านทุกไฟล์ที่เจอแล้ว union รหัสเข้าด้วยกัน → อยู่ร่วมกันได้หลายปี (`_67`, `_69`) โดยไม่ต้องแก้โค้ด · เพิ่ม `old100_years()` ถอดปีจากท้ายชื่อไปต่อท้าย legend ใน README ให้เอง (`โค้ดจากปี 68`) · ไฟล์ marker ในโฟลเดอร์โจทย์ใช้ชื่อเดียวกัน (`.old100_68`) เพื่อให้ทั้งสองที่อ่านตรงกัน · ทดสอบแล้ว: วาง `_69` เพิ่มชั่วคราว → ได้ 40 รหัส (union จริง ไม่ใช่ทับ) และ `years=['68','69']` · ไฟล์ `.bak` ไม่ถูกนับเพราะบังคับ suffix `.txt` · contest ที่ไม่มีป้ายยังไม่มีคอลัมน์โผล่
- **บทเรียนทั่วไป:** ข้อมูลเมตา (ปี/ที่มา/เวอร์ชัน) ที่อยู่ในหัวคน ให้ย้ายไปอยู่ใน**ชื่อไฟล์** แล้วให้โค้ดอ่านจากชื่อ — และหลีกเลี่ยงการผูกชื่อไฟล์แบบตายตัวเมื่ออนาคตมีของแบบเดียวกันเพิ่มได้อีก
- **Date Added:** 2026-10-03
