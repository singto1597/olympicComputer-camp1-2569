#pragma once

struct Node {
    int data;
    Node* next;
};

// กลับลำดับของลิสต์ทั้งลิสต์ โดย "เปลี่ยนลูกศร next และ head เท่านั้น"
// ห้ามแก้ data ห้ามสร้างโหนดใหม่ (new) ห้ามใช้ vector / array / stack เก็บโหนด
// ต้องทำได้ในเวลา O(N) และใช้หน่วยความจำเพิ่ม O(1)
void reverseList(Node*& head);

// กลับลำดับเฉพาะโหนดตำแหน่งที่ l ถึง r (นับจาก 1) โดยโหนดอื่นอยู่ที่เดิม
// รับประกัน 1 <= l <= r <= จำนวนโหนด  เงื่อนไขการแก้ลูกศรเหมือน reverseList
void reverseRange(Node*& head, int l, int r);
