#include "reverse_bridge.h"
// ไฟล์โครง: เขียนตัวฟังก์ชันให้ครบแล้วส่งเฉพาะไฟล์นี้ (ห้ามแก้ไฟล์ .h และไม่ต้องเขียน main)

// กลับลำดับของลิสต์ทั้งลิสต์ โดย "เปลี่ยนลูกศร next และ head เท่านั้น"
// ห้ามแก้ data ห้ามสร้างโหนดใหม่ (new) ห้ามใช้ vector / array / stack เก็บโหนด
// ต้องทำได้ในเวลา O(N) และใช้หน่วยความจำเพิ่ม O(1)

// 1. ว่าง
// 2. หัว
// 3. หาง
// 4. กลาง

void reverseList(Node*& head) {
    if (!head) return;
    Node *new_head = nullptr;
    Node *cur = head;
    while (cur != nullptr){
        Node *temp = cur;
        cur = cur->next;
        temp->next = new_head;
        new_head = temp;
    }
    head = new_head;
    return;
}

// กลับลำดับเฉพาะโหนดตำแหน่งที่ l ถึง r (นับจาก 1) โดยโหนดอื่นอยู่ที่เดิม
// รับประกัน 1 <= l <= r <= จำนวนโหนด  เงื่อนไขการแก้ลูกศรเหมือน reverseList
void reverseRange(Node*& head, int l, int r) {
    if (!head || l == r) return;

    Node *cur = head;
    if (l == 1){
        for (int i = 1; i <= r - 1; i++){
            cur = cur->next;
        }
        Node *end = cur->next;
        cur->next = nullptr;
        reverseList(head);
        cur = head;
        while (cur->next != nullptr) cur = cur->next;
        cur->next = end;
        return;
    }

    for (int i = 1; i <= r - 1; i++){
        cur = cur->next;
    }
    Node *end = cur->next;
    cur->next = nullptr;

    cur = head;
    for (int i = 1; i <= l - 2; i++){
        cur = cur->next;
    }

    Node *start = cur->next;
    cur->next = nullptr;

    reverseList(start);

    cur->next = start;
    while (start->next != nullptr) start = start->next;
    start->next = end;
}
