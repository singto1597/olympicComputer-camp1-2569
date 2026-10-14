#include "twin_scroll.h"
// ไฟล์โครง: เขียนตัวฟังก์ชันให้ครบแล้วส่งเฉพาะไฟล์นี้ (ห้ามแก้ไฟล์ .h และไม่ต้องเขียน main)

// สร้างม้วนคัมภีร์ว่าง (sentinel หนึ่งโหนด) แล้วคืนพอยน์เตอร์ของ sentinel
Rune* makeScroll() {
    // TODO: เขียนโค้ดตรงนี้
    return nullptr;
}

// แทรกตัวอักษร c ทางซ้ายของตำแหน่งเขียน (ก่อนโหนด cursor) cursor ไม่เปลี่ยน
void insertLeft(Rune* sentinel, Rune*& cursor, char c) {
    // TODO: เขียนโค้ดตรงนี้
}

// เลื่อนตำแหน่งเขียนไปซ้าย 1 ตัวอักษร (ถ้าอยู่หน้าสุดแล้วไม่ต้องทำอะไร)
void moveLeft(Rune* sentinel, Rune*& cursor) {
    // TODO: เขียนโค้ดตรงนี้
}

// เลื่อนตำแหน่งเขียนไปขวา 1 ตัวอักษร (ถ้าอยู่ท้ายสุดแล้วไม่ต้องทำอะไร)
void moveRight(Rune* sentinel, Rune*& cursor) {
    // TODO: เขียนโค้ดตรงนี้
}

// ลบและ delete ตัวอักษรทางซ้ายของตำแหน่งเขียน (ถ้ามี) ต้องรักษา prev และ next ให้ถูกต้อง
void eraseLeft(Rune* sentinel, Rune*& cursor) {
    // TODO: เขียนโค้ดตรงนี้
}
