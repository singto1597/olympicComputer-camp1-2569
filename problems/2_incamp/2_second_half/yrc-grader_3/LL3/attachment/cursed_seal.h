#pragma once

struct Node {
    int data;
    Node* next;
};

// ลบโหนดแรกสุดที่มี data == x แล้ว delete โหนดนั้น
// คืน true ถ้าลบสำเร็จ  คืน false ถ้าไม่พบ x (ลิสต์ไม่เปลี่ยน)
// หมายเหตุ: head อาจเปลี่ยน หรืออาจกลายเป็น nullptr ได้
bool removeFirst(Node*& head, int x);

// ลบ "ทุก" โหนดที่มี data == x แล้ว delete ทุกโหนดที่ลบ
// คืนจำนวนโหนดที่ถูกลบ (0 ถ้าไม่พบ x) ต้องทำได้ในเวลา O(N) โดยเดินลิสต์รอบเดียว
int removeAll(Node*& head, int x);
