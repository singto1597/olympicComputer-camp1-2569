#pragma once
#include <vector>

struct Node {
    int data;
    Node* next;
};

// สร้างลิสต์โยงเดี่ยวจากข้อมูลใน values (ตามลำดับ) แล้วคืนพอยน์เตอร์ของโหนดแรก
// ถ้า values ว่าง ให้คืน nullptr
Node* buildPath(const std::vector<int>& values);

// อ่านค่า data ของทุกโหนดตั้งแต่หัวถึงท้าย แล้วคืนเป็นเวกเตอร์ (ห้ามแก้ลิสต์)
std::vector<int> readPath(Node* head);

// คืนพอยน์เตอร์ของโหนดแรกสุดที่มี data == x ถ้าไม่พบให้คืน nullptr (ห้ามแก้ลิสต์)
Node* findJade(Node* head, int x);
