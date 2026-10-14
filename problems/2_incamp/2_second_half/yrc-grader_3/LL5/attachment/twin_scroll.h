#pragma once

// หนึ่งตัวอักษรบนม้วนคัมภีร์ (ลิสต์โยงสองทาง)
struct Rune {
    char data;
    Rune* prev;
    Rune* next;
};

// ม้วนคัมภีร์ใช้ "sentinel" เป็นโหนดพิเศษที่ไม่ใช่ตัวอักษร ต่อกันเป็นวงกลม:
//   sentinel->next คือตัวอักษรแรก  และ  sentinel->prev คือตัวอักษรสุดท้าย
//   ม้วนว่างคือ sentinel ที่ prev และ next ชี้ตัวเองทั้งคู่
// cursor คือ "โหนดที่อยู่ทางขวาของตำแหน่งเขียน" ถ้าตำแหน่งเขียนอยู่ท้ายสุด cursor == sentinel

// สร้างม้วนคัมภีร์ว่าง (sentinel หนึ่งโหนด) แล้วคืนพอยน์เตอร์ของ sentinel
Rune* makeScroll();

// แทรกตัวอักษร c ทางซ้ายของตำแหน่งเขียน (ก่อนโหนด cursor) cursor ไม่เปลี่ยน
void insertLeft(Rune* sentinel, Rune*& cursor, char c);

// เลื่อนตำแหน่งเขียนไปซ้าย 1 ตัวอักษร (ถ้าอยู่หน้าสุดแล้วไม่ต้องทำอะไร)
void moveLeft(Rune* sentinel, Rune*& cursor);

// เลื่อนตำแหน่งเขียนไปขวา 1 ตัวอักษร (ถ้าอยู่ท้ายสุดแล้วไม่ต้องทำอะไร)
void moveRight(Rune* sentinel, Rune*& cursor);

// ลบและ delete ตัวอักษรทางซ้ายของตำแหน่งเขียน (ถ้ามี) ต้องรักษา prev และ next ให้ถูกต้อง
void eraseLeft(Rune* sentinel, Rune*& cursor);
