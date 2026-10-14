// grader.cpp — โปรแกรมตรวจของข้อ hero_caravan (CMS: Managers)
#include "hero_caravan.h"
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <new>
#include <vector>
#include <string>
#include <list>
#include <unordered_map>
#include <algorithm>
using namespace std;

// ============ ส่วนตรวจหน่วยความจำของโปรแกรมตรวจ (ผู้เรียนไม่ต้องอ่านก็ได้) ============
// ทุกครั้งที่เรียก new จะมีป้ายเล็ก ๆ ติดไว้หน้าหน่วยความจำ เพื่อให้ตรวจได้ว่า
//  - โหนดถูกสร้างด้วย new จริง   - โหนดที่ลบออกถูก delete แล้ว   - ไม่มี delete ซ้ำ
namespace chk {
struct alignas(16) Hdr { uint64_t magic; uint64_t state; };
const uint64_t MAGIC = 0x4C4C4953544D4147ULL;
long long allocs = 0;    // จำนวนครั้งที่เรียก new ทั้งหมด
long long badFree = 0;   // จำนวนครั้งที่ delete ผิดปกติ (ซ้ำ หรือไม่ใช่ของที่มาจาก new)
}
__attribute__((noinline)) void* operator new(size_t n) {
    ++chk::allocs;
    void* raw = std::malloc(n + sizeof(chk::Hdr));
    if (!raw) throw std::bad_alloc();
    chk::Hdr* h = (chk::Hdr*)raw; h->magic = chk::MAGIC; h->state = 0;
    return (char*)raw + sizeof(chk::Hdr);
}
__attribute__((noinline)) void operator delete(void* p) noexcept {
    if (!p) return;
    chk::Hdr* h = (chk::Hdr*)((uintptr_t)p - sizeof(chk::Hdr));
    if (h->magic != chk::MAGIC || h->state != 0) { ++chk::badFree; return; }
    h->state = 1;        // ทำเครื่องหมายว่าคืนแล้ว (ไม่คืนจริง เพื่อให้ตรวจย้อนหลังได้)
}
void operator delete(void* p, size_t) noexcept { operator delete(p); }

static inline const chk::Hdr* hdrOf(const void* p) { return (const chk::Hdr*)((uintptr_t)p - sizeof(chk::Hdr)); }
static inline bool isLive(const void* p)  { return p && hdrOf(p)->magic == chk::MAGIC && hdrOf(p)->state == 0; }
static inline bool isFreed(const void* p) { return p && hdrOf(p)->magic == chk::MAGIC && hdrOf(p)->state == 1; }
[[noreturn]] static void invalid(const char* why) {
    std::printf("INVALID\n");
    std::fprintf(stderr, "INVALID: %s\n", why);
    std::fflush(stdout);
    std::exit(0);
}
static void outInt(string& s, long long v) { s += std::to_string(v); }
static void flushOut(string& s) { std::fwrite(s.data(), 1, s.size(), stdout); s.clear(); }
static int rdInt() { int x; if (std::scanf("%d", &x) != 1) std::exit(0); return x; }
[[maybe_unused]] static char rdChar() { char c; if (std::scanf(" %c", &c) != 1) std::exit(0); return c; }
[[maybe_unused]] static string rdStr() { string r; int c = std::getchar(); while (c == ' ' || c == '\n' || c == '\r') c = std::getchar(); while (c != EOF && c != ' ' && c != '\n' && c != '\r') { r += (char)c; c = std::getchar(); } return r; }

static void light(const List& L, int sz) {
    if (L.size != sz) invalid("L.size ไม่ถูกต้อง");
    if (sz == 0) { if (L.head || L.tail) invalid("ลิสต์ว่างต้องมี head และ tail เป็น nullptr"); return; }
    if (!L.head || !L.tail) invalid("ลิสต์ไม่ว่างแต่ head หรือ tail เป็น nullptr");
    if (L.tail->next != nullptr) invalid("tail->next ต้องเป็น nullptr");
    if (sz == 1 && L.head != L.tail) invalid("ลิสต์มีโหนดเดียว head ต้องเท่ากับ tail");
}
int main() {
    int Q = rdInt(); string out;
    while (Q--) {
        int n = rdInt(), m = rdInt();
        List L{nullptr, nullptr, 0}; list<int> md;
        for (int i = 0; i < n; i++) {
            int v = rdInt(); Node* nd = new Node{v, nullptr};
            if (!L.head) L.head = L.tail = nd; else { L.tail->next = nd; L.tail = nd; }
            L.size++; md.push_back(v);
        }
        string res; int na = 0;
        for (int i = 0; i < m; i++) {
            char op = rdChar();
            if (op == 'F') {
                int x = rdInt(); pushFront(L, x); md.push_front(x);
                light(L, (int)md.size()); if (L.head->data != x) invalid("pushFront: โหนดแรกต้องมีค่า x");
            } else if (op == 'B') {
                int x = rdInt(); pushBack(L, x); md.push_back(x);
                light(L, (int)md.size()); if (L.tail->data != x) invalid("pushBack: โหนดสุดท้ายต้องมีค่า x");
            } else {
                int t = rdInt(), x = rdInt();
                bool r = insertAfter(L, t, x);
                auto it = std::find(md.begin(), md.end(), t); bool e = (it != md.end());
                if (e) md.insert(std::next(it), x);
                if (r != e) invalid("insertAfter คืนค่าผิด");
                light(L, (int)md.size());
                res += (na++ ? " " : ""); res += r ? '1' : '0';
            }
        }
        // ตรวจโครงสร้างทั้งลิสต์
        vector<Node*> nodes; auto it = md.begin();
        for (Node* p = L.head; p; p = p->next, ++it) {
            if (nodes.size() >= md.size()) invalid("ลิสต์ยาวเกินจริงหรือมีวงวน");
            if (!isLive(p)) invalid("โหนดต้องสร้างด้วย new และห้ามถูก delete");
            if (p->data != *it) invalid("ค่าหรือลำดับของโหนดผิด");
            nodes.push_back(p);
        }
        if (nodes.size() != md.size()) invalid("จำนวนโหนดไม่ตรง");
        if (!nodes.empty() && nodes.back() != L.tail) invalid("tail ไม่ได้ชี้โหนดสุดท้ายจริง");
        outInt(out, na); if (na) { out += ' '; out += res; } out += '\n';
        outInt(out, (long long)nodes.size()); for (Node* p : nodes) { out += ' '; outInt(out, p->data); } out += '\n';
        for (Node* p : nodes) delete p;
        if (chk::badFree) invalid("delete ผิดปกติ");
        flushOut(out);
    }
    return 0;
}
