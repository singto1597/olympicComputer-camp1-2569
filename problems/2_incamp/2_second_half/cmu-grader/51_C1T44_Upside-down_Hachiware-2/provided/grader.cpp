#include "C1T44.h"
using namespace std;

using ll = long long;

const int MAXN = 5000;
ll a[MAXN + 5];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    cin >> N >> K;

    for (int i = 0; i < N; ++i) {
        cin >> a[i];
    }

    ll result = hachiware_bubble(a, N, K);

    cout << result << '\n';

    for (int i = 0; i < N; ++i) {
        cout << a[i] << ' ';
    }

    cout << '\n';

    return 0;
}