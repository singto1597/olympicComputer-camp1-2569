# ตราคำสาป (cursed_seal) - ไฟล์แนบ

นักเรียนเขียนฟังก์ชันลงใน `cursed_seal.cpp` (ไฟล์โครง) โดยไม่แก้ `cursed_seal.h` และส่งเฉพาะ `cursed_seal.cpp`

`grader.cpp` คือโปรแกรมตรวจเดียวกับที่ระบบใช้ อ่านอินพุตมาตรฐาน เรียกฟังก์ชันของคุณ และตรวจกติกา (การใช้ `new`/`delete`, โหนดต้องเป็นโหนดเดิม ฯลฯ) ถ้าผิดกติกาจะพิมพ์ `INVALID`

```sh
chmod +x compile.sh && ./compile.sh      # คอมไพล์ แล้วรันกับ sample/ ทุกชุด
```

หรือคอมไพล์เอง: `g++ -std=c++17 -O2 grader.cpp cursed_seal.cpp -o cursed_seal` แล้ว `./cursed_seal < sample/sample1.in`
ผลลัพธ์ต้องตรงกับ `sample/sampleN.out` (เทียบแบบไม่สนช่องว่าง) ข้อมูลใน sample ไม่ใช่ข้อมูลที่ใช้ให้คะแนน
