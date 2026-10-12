#include "n_pens.h"
#include <iostream>
int main() {
    long long n, k;
    if (!(std::cin >> n >> k) || n < 1 || n > 200000 || k < 1 || k > 1000000000000000000LL) {
        std::cerr << "Invalid sample input\n"; return 2;
    }
    int order = choose_turn(n, k);
    if (order != 1 && order != 2) { std::cout << "0\n"; return 0; }
    bool contestant = order == 1;
    while (n) {
        long long take;
        if (contestant) take = take_pens(n, k);
        else if (!(std::cin >> take)) {
            std::cerr << "Missing opponent move\n"; return 2;
        }
        if (take < 1 || take > k || take > n) {
            if (!contestant) { std::cerr << "Illegal scripted opponent move\n"; return 2; }
            std::cout << "0\n"; return 0;
        }
        n -= take;
        if (!n) { std::cout << (contestant ? 0 : 1) << '\n'; return 0; }
        contestant = !contestant;
    }
}
