# ขบวนผู้กล้า (hero_caravan) - ไฟล์แนบ

นักเรียนเขียนฟังก์ชันลงใน `hero_caravan.cpp` (ไฟล์โครง) โดยไม่แก้ `hero_caravan.h` และส่งเฉพาะ `hero_caravan.cpp`

`grader.cpp` คือโปรแกรมตรวจเดียวกับที่ระบบใช้ อ่านอินพุตมาตรฐาน เรียกฟังก์ชันของคุณ และตรวจกติกา (การใช้ `new`/`delete`, โหนดต้องเป็นโหนดเดิม ฯลฯ) ถ้าผิดกติกาจะพิมพ์ `INVALID`

```sh
chmod +x compile.sh && ./compile.sh      # คอมไพล์ แล้วรันกับ sample/ ทุกชุด
```

หรือคอมไพล์เอง: `g++ -std=c++17 -O2 grader.cpp hero_caravan.cpp -o hero_caravan` แล้ว `./hero_caravan < sample/sample1.in`
ผลลัพธ์ต้องตรงกับ `sample/sampleN.out` (เทียบแบบไม่สนช่องว่าง) ข้อมูลใน sample ไม่ใช่ข้อมูลที่ใช้ให้คะแนน
