#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int> b(n);
    vector<vector<int>> a(m, vector<int>(m));
    vector<vector<int>> dp(m, vector<int>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < m; k++) {
                cin >> a[j][k];

                if (j == 0 && k == 0) {
                    dp[j][k] = a[j][k];
                } else if (j == 0) {
                    dp[j][k] = dp[j][k - 1] + a[j][k];
                } else if (k == 0) {
                    dp[j][k] = dp[j - 1][k] + a[j][k];
                } else {
                    dp[j][k] = min(dp[j - 1][k], dp[j][k - 1]) + a[j][k];
                }
            }
        }
        b[i] = dp[m - 1][m - 1];
    }

    int q;
    cin >> q;
    while (q--) {
        long long x;
        cin >> x;
        bool flag = true;

        for (int i = 0; i < n; i++) {
            x -= b[i];
            if (x < 0) {
                flag = false;
                cout << i + 1 << "\n";
                break;
            }
        }
        if (flag) {
            cout << "Cleared\n";
        }
    }

    return 0;
}
