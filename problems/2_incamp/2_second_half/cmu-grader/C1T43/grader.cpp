#include "C1T43.h"
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    long long energy[N + 1] = {};

    for (int i = 1; i <= N; ++i) {
        cin >> energy[i];
    }

    cout << maximum_kuromi_energy(energy, N) << "\n";

    return 0;
}