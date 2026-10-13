#include "div_mod.h"
#include <iostream>

#define NEED(expr) \
    if (!(expr)) { \
        std::cerr << "Missing input for call " << (t + 1) << "\n"; \
        return 2; \
    }

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int Q;
    if (!(std::cin >> Q) || Q <= 0) {
        std::cerr << "Invalid or missing Q\n";
        return 2;
    }
    for (int t = 0; t < Q; ++t) {
        int a, b;
        NEED(std::cin >> a >> b);
        int q = 2147483647, r = 2147483647;
        divMod(a, b, q, r);
        std::cout << q << " " << r << "\n";
    }
    return 0;
}
