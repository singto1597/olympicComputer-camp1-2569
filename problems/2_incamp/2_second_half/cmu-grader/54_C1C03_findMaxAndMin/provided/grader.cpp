#include "find_max_and_min.h"
#include <iostream>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int Q;
    if (!(std::cin >> Q) || Q <= 0) {
        std::cerr << "Invalid or missing Q\n";
        return 2;
    }
    for (int q = 0; q < Q; ++q) {
        int N;
        if (!(std::cin >> N) || N <= 0) {
            std::cerr << "Invalid or missing N for call " << (q + 1) << "\n";
            return 2;
        }
        std::vector<int> A(N);
        for (int i = 0; i < N; ++i) {
            if (!(std::cin >> A[i])) {
                std::cerr << "Missing element at index " << i << " for call " << (q + 1) << "\n";
                return 2;
            }
        }
        std::pair<int, int> ans = findMaxAndMin(A);
        std::cout << ans.first << " " << ans.second << "\n";
    }
    return 0;
}
