#include "hero_caravan.h"
// ไฟล์โครง: เขียนตัวฟังก์ชันให้ครบแล้วส่งเฉพาะไฟล์นี้ (ห้ามแก้ไฟล์ .h และไม่ต้องเขียน main)

// สร้างโหนดค่า x ไว้หน้าสุดของลิสต์ (ลิสต์ว่างก็ใช้ได้)
void pushFront(List& L, int x) {
    // TODO: เขียนโค้ดตรงนี้


    Node *newNode = new Node();

    newNode->data = x;
    newNode->next = nullptr;

    if (L.size == 0){
        L.head = newNode;
        L.tail = newNode;
        L.size++;
        return;
    }

    L.size++;

    newNode->next = L.head;

    L.head = newNode;
    return;
}

// สร้างโหนดค่า x ไว้ท้ายสุดของลิสต์ (ลิสต์ว่างก็ใช้ได้) ต้องทำได้ในเวลา O(1)
void pushBack(List& L, int x) {
    // TODO: เขียนโค้ดตรงนี้
    Node *newNode = new Node();

    newNode->data = x;
    newNode->next = nullptr;

    if (L.size == 0){
        L.head = newNode;
        L.tail = newNode;
        L.size++;
        return;
    }

    L.size++;

    L.tail->next = newNode;

    L.tail = newNode;
    return;
}

// แทรกโหนดค่า x ไว้หลังโหนดแรกที่มี data == target
// คืน true ถ้าแทรกสำเร็จ ถ้าไม่พบ target ให้คืน false และไม่เปลี่ยนลิสต์
bool insertAfter(List& L, int target, int x) {
    // TODO: เขียนโค้ดตรงนี้

    if (L.size == 0){
        return false;
    }

    Node *temp = L.head;

    Node *newNode = new Node();

    newNode->data = x;
    newNode->next = nullptr;

    while(temp->next != nullptr && temp->data != target){
        temp = temp->next;
    }

    if (temp->data == target){
        newNode->next = temp->next;
        temp->next = newNode;
        if (newNode->next == nullptr){
            L.tail = newNode;
        }
        L.size++;
        return true;
    }


    return false;
}
