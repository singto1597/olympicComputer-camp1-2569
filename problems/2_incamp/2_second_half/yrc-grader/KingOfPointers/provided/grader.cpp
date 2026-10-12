#include <iostream>

#include "set_pointer.h"

int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);

    int q;
    if (!(std::cin >> q)) {
        return 0;
    }

    for (int k = 0; k < q; ++k) {
        int x;
        std::cin >> x;

        int actual_value = 0;
        const int *p1 = &actual_value;
        const int **p2 = &p1;
        const int ***ptr = &p2;

        setPointer(ptr, x);
        std::cout << actual_value << "\n";
    }
    return 0;
}
