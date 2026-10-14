#include "jade_path.h"
// ไฟล์โครง: เขียนตัวฟังก์ชันให้ครบแล้วส่งเฉพาะไฟล์นี้ (ห้ามแก้ไฟล์ .h และไม่ต้องเขียน main)

using namespace std;

// สร้างลิสต์โยงเดี่ยวจากข้อมูลใน values (ตามลำดับ) แล้วคืนพอยน์เตอร์ของโหนดแรก
// ถ้า values ว่าง ให้คืน nullptr


Node* buildPath(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    Node* head = nullptr;


    Node *temp = head;

    for (int value : values){
        Node *newNode = new Node{value, nullptr};
        if (!head){
            head = newNode;
            temp = newNode;
            continue;
        }

        temp->next = newNode;
        temp = newNode;


    }

    return head;
}

// อ่านค่า data ของทุกโหนดตั้งแต่หัวถึงท้าย แล้วคืนเป็นเวกเตอร์ (ห้ามแก้ลิสต์)
std::vector<int> readPath(Node* head) {
    vector<int> list_vec;
    if (!head){
        return list_vec;
    }

    Node *temp = head;

    while (temp){
        list_vec.push_back(temp->data);
        temp = temp->next;
    }


    return list_vec;
}

// คืนพอยน์เตอร์ของโหนดแรกสุดที่มี data == x ถ้าไม่พบให้คืน nullptr (ห้ามแก้ลิสต์)
Node* findJade(Node* head, int x) {
    // TODO: เขียนโค้ดตรงนี้
    Node *temp = head;

    while (temp){
        if (temp->data == x){
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}
