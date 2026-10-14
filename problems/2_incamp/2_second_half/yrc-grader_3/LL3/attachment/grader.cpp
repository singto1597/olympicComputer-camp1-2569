// grader.cpp — โปรแกรมตรวจของข้อ cursed_seal (CMS: Managers)
#include "cursed_seal.h"
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
        vector<int> val(n); vector<Node*> orig(n);
        Node* head = nullptr; Node* tail = nullptr;
        for (int i = 0; i < n; i++) {
            val[i] = rdInt(); orig[i] = new Node{val[i], nullptr};
            if (tail) tail->next = orig[i]; else head = orig[i];
            tail = orig[i];
        }
        vector<int> alive(n); for (int i = 0; i < n; i++) alive[i] = i;    // ดัชนีโหนดที่ยังไม่ถูกลบ (ตามลำดับ)
        vector<char> dead(n, 0);
        vector<int> res;
        for (int i = 0; i < m; i++) {
            char op = rdChar(); int x = rdInt(); long long a0 = chk::allocs, a1 = a0;
            if (op == 'F') {
                bool r = removeFirst(head, x); a1 = chk::allocs;
                size_t pos = 0; while (pos < alive.size() && val[alive[pos]] != x) pos++;
                bool e = pos < alive.size();
                if (e) { dead[alive[pos]] = 1; alive.erase(alive.begin() + pos); }
                if (r != e) invalid("removeFirst คืนค่าผิด");
                res.push_back(r ? 1 : 0);
            } else {
                int r = removeAll(head, x); a1 = chk::allocs;
                vector<int> keep; int cnt = 0;
                for (int id : alive) { if (val[id] == x) { dead[id] = 1; cnt++; } else keep.push_back(id); }
                alive.swap(keep);
                if (r != cnt) invalid("removeAll คืนจำนวนผิด");
                res.push_back(r);
            }
            if (a1 != a0) invalid("การลบไม่ต้องสร้างโหนดใหม่ (ห้ามใช้ new)");
        }
        // ตรวจลิสต์สุดท้าย: ต้องเป็นโหนดเดิมที่ยังไม่ถูกลบ ตามลำดับเดิมเป๊ะ
        size_t k = 0;
        for (Node* p = head; p; p = p->next, k++) {
            if (k >= alive.size() || p != orig[alive[k]]) invalid("ลิสต์สุดท้ายไม่ตรงกับโหนดที่ควรเหลือ (ตรวจการเชื่อมลูกศร)");
        }
        if (k != alive.size()) invalid("จำนวนโหนดที่เหลือไม่ตรง");
        for (int i = 0; i < n; i++) {
            if (dead[i] && !isFreed(orig[i])) invalid("โหนดที่ถูกลบต้องถูก delete");
            if (!dead[i] && !isLive(orig[i])) invalid("โหนดที่ไม่ได้ถูกลบห้ามถูก delete");
        }
        if (chk::badFree) invalid("delete ซ้ำหรือ delete สิ่งที่ไม่ได้มาจาก new");
        outInt(out, m); for (int r : res) { out += ' '; outInt(out, r); } out += '\n';
        outInt(out, (long long)alive.size()); for (int id : alive) { out += ' '; outInt(out, val[id]); } out += '\n';
        for (int id : alive) delete orig[id];
        chk::badFree = 0;
        flushOut(out);
    }
    return 0;
}
