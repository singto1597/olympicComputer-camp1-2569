#include "exchange.h"
#include <iostream>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long X;
    int N;
    if (!(std::cin >> X >> N)) {
        return 0;
    }

    std::vector<long long> A(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> A[i];
    }

    std::vector<long long> result = exchange(X, A);

    if (result.size() != static_cast<size_t>(N)) {
        std::cout << "INVALID_SIZE\n";
        return 0;
    }

    for (int i = 0; i < N; ++i) {
        if (i > 0) {
            std::cout << " ";
        }
        std::cout << result[i];
    }
    std::cout << "\n";

    return 0;
}
