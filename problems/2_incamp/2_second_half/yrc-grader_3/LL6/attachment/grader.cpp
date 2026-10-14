// grader.cpp — โปรแกรมตรวจของข้อ demon_ring (CMS: Managers)
#include "demon_ring.h"
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
        int n = rdInt(), k = rdInt();
        Node* c = buildRing(n);
        vector<Node*> ptr(n); Node* p = c;
        for (int i = 0; i < n; i++) {
            if (!p || !isLive(p)) invalid("buildRing: โหนดต้องสร้างด้วย new และห้าม nullptr กลางวง");
            if (p->data != i + 1) invalid("buildRing: ต้องเรียงหมายเลข 1..n และคืนโหนดหมายเลข 1");
            ptr[i] = p; p = p->next;
        }
        if (p != c) invalid("buildRing: next ของโหนดสุดท้ายต้องชี้กลับโหนดแรก (วงกลม)");
        vector<int> res = breakRing(c, k);
        if ((int)res.size() != n - 1) invalid("breakRing ต้องคืนค่า n-1 ค่า");
        vector<char> gone(n + 1, 0);
        for (int id : res) { if (id < 1 || id > n || gone[id]) invalid("ค่าที่คืนไม่ถูกต้องหรือซ้ำ"); gone[id] = 1; }
        if (!c || !isLive(c)) invalid("current ต้องชี้โหนดที่รอด");
        int sv = c->data;
        if (sv < 1 || sv > n || ptr[sv - 1] != c || gone[sv]) invalid("current ต้องชี้โหนดที่รอด (ไม่ใช่โหนดที่ถูกลบ)");
        if (c->next != c) invalid("โหนดที่รอดต้องชี้ next ไปที่ตัวเอง");
        for (int i = 1; i <= n; i++) {
            if (gone[i] && !isFreed(ptr[i - 1])) invalid("โหนดที่ถูกลบต้องถูก delete");
            if (!gone[i] && !isLive(ptr[i - 1])) invalid("โหนดที่รอดห้ามถูก delete");
        }
        if (chk::badFree) invalid("delete ซ้ำหรือ delete สิ่งที่ไม่ได้มาจาก new");
        outInt(out, (long long)res.size()); for (int id : res) { out += ' '; outInt(out, id); } out += '\n';
        outInt(out, sv); out += '\n';
        delete c;
        flushOut(out);
    }
    return 0;
}
