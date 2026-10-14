// grader.cpp — โปรแกรมตรวจของข้อ jade_path (CMS: Managers)
#include "jade_path.h"
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

static vector<Node*> walkExact(Node* head, const vector<int>& a) {
    vector<Node*> v; size_t n = a.size();
    for (Node* p = head; p; p = p->next) {
        if (v.size() >= n) invalid("ลิสต์ยาวกว่าจำนวนข้อมูล หรือมีวงวน (ท้ายลิสต์ต้องเป็น nullptr)");
        if (!isLive(p)) invalid("โหนดต้องสร้างด้วย new");
        v.push_back(p);
    }
    if (v.size() != n) invalid("จำนวนโหนดไม่ตรงกับจำนวนข้อมูล");
    for (size_t i = 0; i < n; i++) if (v[i]->data != a[i]) invalid("ค่าหรือลำดับของโหนดไม่ตรงกับข้อมูล");
    return v;
}
static void sameNodes(Node* head, const vector<Node*>& nodes) {
    size_t i = 0;
    for (Node* p = head; p; p = p->next, i++) if (i >= nodes.size() || p != nodes[i]) invalid("ฟังก์ชันนี้ไม่ควรเปลี่ยนลิสต์");
    if (i != nodes.size()) invalid("ฟังก์ชันนี้ไม่ควรเปลี่ยนลิสต์");
}
int main() {
    int Q = rdInt();
    string out;
    while (Q--) {
        int n = rdInt(); vector<int> a(n);
        for (int& x : a) x = rdInt();
        int k = rdInt(); vector<int> qs(k);
        for (int& x : qs) x = rdInt();

        Node* head = buildPath(a);
        vector<Node*> nodes = walkExact(head, a);

        vector<int> r = readPath(head);
        sameNodes(head, nodes);
        outInt(out, (long long)r.size());
        for (int x : r) { out += ' '; outInt(out, x); }
        out += '\n';

        unordered_map<Node*, int> idx;
        if (k > 0) { idx.reserve(nodes.size() * 2 + 1); for (size_t i = 0; i < nodes.size(); i++) idx[nodes[i]] = (int)i; }
        for (int j = 0; j < k; j++) {
            Node* p = findJade(head, qs[j]);
            int ans = -1;
            if (p) { auto it = idx.find(p); if (it == idx.end()) invalid("findJade คืนพอยน์เตอร์ที่ไม่ใช่โหนดในลิสต์"); ans = it->second; }
            if (j) out += ' ';
            outInt(out, ans);
        }
        out += '\n';
        sameNodes(head, nodes);
        for (Node* p : nodes) delete p;
        if (chk::badFree) invalid("delete ผิดปกติ");
        flushOut(out);
    }
    flushOut(out);
    return 0;
}
