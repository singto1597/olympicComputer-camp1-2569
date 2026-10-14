// grader.cpp — โปรแกรมตรวจของข้อ reverse_bridge (CMS: Managers)
#include "reverse_bridge.h"
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

static void verifyOrder(Node* head, const vector<Node*>& ord) {
    size_t k = 0;
    for (Node* p = head; p; p = p->next, k++) {
        if (k >= ord.size() || p != ord[k]) invalid("ลำดับโหนดหลังกลับไม่ตรง (ต้องกลับโหนดจริง ไม่ใช่สลับค่า data)");
    }
    if (k != ord.size()) invalid("จำนวนโหนดหลังกลับไม่ตรง (มีโหนดหาย)");
}
int main() {
    int Q = rdInt(); string out;
    while (Q--) {
        int n = rdInt(), m = rdInt();
        vector<int> val(n); vector<Node*> ord(n); Node* head = nullptr; Node* tail = nullptr;
        for (int i = 0; i < n; i++) {
            val[i] = rdInt(); ord[i] = new Node{val[i], nullptr};
            if (tail) tail->next = ord[i]; else head = ord[i];
            tail = ord[i];
        }
        vector<Node*> all = ord;
        for (int i = 0; i < m; i++) {
            char op = rdChar(); long long a0 = chk::allocs;
            if (op == 'W') { reverseList(head); std::reverse(ord.begin(), ord.end()); }
            else { int l = rdInt(), r = rdInt(); reverseRange(head, l, r); std::reverse(ord.begin() + (l - 1), ord.begin() + r); }
            if (chk::allocs != a0) invalid("ห้ามสร้างโหนดใหม่หรือใช้ vector/string ในฟังก์ชันกลับลำดับ");
            verifyOrder(head, ord);
        }
        for (Node* p : all) if (!isLive(p)) invalid("ห้าม delete โหนดใด ๆ");
        if (chk::badFree) invalid("delete ผิดปกติ");
        outInt(out, n); for (Node* p : ord) { out += ' '; outInt(out, p->data); } out += '\n';
        for (Node* p : all) delete p;
        flushOut(out);
    }
    return 0;
}
