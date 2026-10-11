#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);cin.tie(NULL);
    long long n, q;
    cin >> n >> q;

    long long a[n + 1] = {};
    for (int i = 1; i <= n; i++) cin >> a[i];

    for (int _ = 0; _ < q; _++){
        long long op, l, r, x;
        cin >> op;
        cin >> l >> r;
        if (op == 1 || op == 2){
            cin >> x;
        }
        unordered_set<int> count_op3;
        int count_op4 = 0;
        switch (op){
            case 1:
                for (long long i = l; i <= r; i++) a[i] += x;
                break;

            case 2:
                for (long long i = l; i <= r; i++) a[i] = x;
                break;

            case 3:
                for (long long i = l; i <= r; i++){
                    // cout << "aaa" << endl;
                    count_op3.insert(a[i]);
                }
                cout << count_op3.size() << endl;
                break;

            case 4:

                for (long long i = l; i < r; i++){
                    for (long long j = i + 1; j <= r; j++)
                    if (a[i] == a[j]) count_op4 ++;
                }
                cout << count_op4 << endl;
                break;



        }

    }




}
