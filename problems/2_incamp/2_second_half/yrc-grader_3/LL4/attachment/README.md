# สะพานย้อนวิถี (reverse_bridge) - ไฟล์แนบ

นักเรียนเขียนฟังก์ชันลงใน `reverse_bridge.cpp` (ไฟล์โครง) โดยไม่แก้ `reverse_bridge.h` และส่งเฉพาะ `reverse_bridge.cpp`

`grader.cpp` คือโปรแกรมตรวจเดียวกับที่ระบบใช้ อ่านอินพุตมาตรฐาน เรียกฟังก์ชันของคุณ และตรวจกติกา (การใช้ `new`/`delete`, โหนดต้องเป็นโหนดเดิม ฯลฯ) ถ้าผิดกติกาจะพิมพ์ `INVALID`

```sh
chmod +x compile.sh && ./compile.sh      # คอมไพล์ แล้วรันกับ sample/ ทุกชุด
```

หรือคอมไพล์เอง: `g++ -std=c++17 -O2 grader.cpp reverse_bridge.cpp -o reverse_bridge` แล้ว `./reverse_bridge < sample/sample1.in`
ผลลัพธ์ต้องตรงกับ `sample/sampleN.out` (เทียบแบบไม่สนช่องว่าง) ข้อมูลใน sample ไม่ใช่ข้อมูลที่ใช้ให้คะแนน
