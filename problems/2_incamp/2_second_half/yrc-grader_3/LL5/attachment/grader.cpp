// grader.cpp — โปรแกรมตรวจของข้อ twin_scroll (CMS: Managers)
#include "twin_scroll.h"
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

int main() {
    int Q = rdInt(); string out;
    while (Q--) {
        int n = rdInt(), m = rdInt();
        string init = n > 0 ? rdStr() : string();
        Rune* s = makeScroll();
        if (!s || !isLive(s) || s->next != s || s->prev != s) invalid("makeScroll ต้องคืน sentinel ที่ prev และ next ชี้ตัวเองทั้งคู่ (สร้างด้วย new)");
        Rune* cursor = s;
        vector<char> L, R;                  // L = ตัวอักษรทางซ้ายตำแหน่งเขียน, R = ทางขวา (เก็บกลับด้าน: back() คือตัวที่ติดตำแหน่งเขียน)
        for (char c : init) { insertLeft(s, cursor, c); L.push_back(c); }
        auto local = [&]() {
            Rune* c = cursor;
            if (!c || !isLive(c)) invalid("cursor ชี้โหนดที่ไม่มีอยู่จริงหรือถูก delete ไปแล้ว");
            if (!c->next || !c->prev || c->next->prev != c || c->prev->next != c) invalid("prev/next ไม่สอดคล้องกัน");
            if ((c == s) != R.empty()) invalid("ตำแหน่ง cursor ไม่ตรงกับตำแหน่งเขียนที่ควรเป็น");
            if (!R.empty() && c->data != R.back()) invalid("cursor ชี้ตัวอักษรผิด");
            if ((c->prev == s) != L.empty()) invalid("ตัวอักษรทางซ้ายของ cursor ไม่ตรง");
            if (!L.empty() && c->prev->data != L.back()) invalid("ตัวอักษรทางซ้ายของ cursor ผิด");
        };
        local();
        for (int i = 0; i < m; i++) {
            char op = rdChar(); long long a0 = chk::allocs, a1;
            if (op == 'P') {
                char c = rdChar(); insertLeft(s, cursor, c); a1 = chk::allocs; L.push_back(c);
                if (a1 != a0 + 1) invalid("insertLeft ต้องสร้างโหนดใหม่ (new) เพียง 1 โหนด");
            } else if (op == 'L') {
                moveLeft(s, cursor); a1 = chk::allocs; if (!L.empty()) { R.push_back(L.back()); L.pop_back(); }
                if (a1 != a0) invalid("moveLeft ไม่ต้องสร้างโหนดใหม่");
            } else if (op == 'R') {
                moveRight(s, cursor); a1 = chk::allocs; if (!R.empty()) { L.push_back(R.back()); R.pop_back(); }
                if (a1 != a0) invalid("moveRight ไม่ต้องสร้างโหนดใหม่");
            } else {
                Rune* victim = (cursor && cursor->prev != s) ? cursor->prev : nullptr;
                eraseLeft(s, cursor); a1 = chk::allocs; if (!L.empty()) L.pop_back();
                if (a1 != a0) invalid("eraseLeft ไม่ต้องสร้างโหนดใหม่");
                if (victim && !isFreed(victim)) invalid("ตัวอักษรที่ลบต้องถูก delete");
            }
            local();
        }
        if (!isLive(s)) invalid("ห้าม delete sentinel");
        // ตรวจทั้งม้วน
        vector<Rune*> ptr; string text; int caret = -1;
        for (Rune* p = s->next; p != s; p = p->next) {
            if (ptr.size() > L.size() + R.size()) invalid("ม้วนยาวเกินจริงหรือเกิดวงวนที่ไม่ผ่าน sentinel");
            if (!isLive(p)) invalid("โหนดต้องสร้างด้วย new และห้ามถูก delete");
            if (p->next->prev != p) invalid("prev ของโหนดถัดไปไม่ชี้กลับมา");
            if (p == cursor) caret = (int)ptr.size();
            ptr.push_back(p); text += p->data;
        }
        if (s->prev != (ptr.empty() ? s : ptr.back())) invalid("sentinel->prev ต้องชี้ตัวอักษรสุดท้าย");
        if (cursor == s) caret = (int)ptr.size();
        string expect(L.begin(), L.end()); expect.append(R.rbegin(), R.rend());
        if (text != expect) invalid("ข้อความไม่ตรง");
        if (caret != (int)L.size()) invalid("ตำแหน่ง cursor ไม่ตรง");
        if (chk::badFree) invalid("delete ผิดปกติ");
        outInt(out, (long long)text.size()); out += ' '; outInt(out, caret); out += ' '; out += text.empty() ? "-" : text; out += '\n';
        for (Rune* p : ptr) delete p;
        delete s;
        flushOut(out);
    }
    return 0;
}
