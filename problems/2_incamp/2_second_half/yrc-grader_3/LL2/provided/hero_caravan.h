#pragma once

struct Node {
    int data;
    Node* next;
};

// รายชื่อผู้กล้าในขบวน เก็บทั้งหัวและท้ายไว้พร้อมกัน
struct List {
    Node* head;   // โหนดแรก (nullptr ถ้าลิสต์ว่าง)
    Node* tail;   // โหนดสุดท้าย (nullptr ถ้าลิสต์ว่าง)
    int size;     // จำนวนโหนดในลิสต์
};

// สร้างโหนดค่า x ไว้หน้าสุดของลิสต์ (ลิสต์ว่างก็ใช้ได้)
void pushFront(List& L, int x);

// สร้างโหนดค่า x ไว้ท้ายสุดของลิสต์ (ลิสต์ว่างก็ใช้ได้) ต้องทำได้ในเวลา O(1)
void pushBack(List& L, int x);

// แทรกโหนดค่า x ไว้หลังโหนดแรกที่มี data == target
// คืน true ถ้าแทรกสำเร็จ ถ้าไม่พบ target ให้คืน false และไม่เปลี่ยนลิสต์
bool insertAfter(List& L, int target, int x);
