#include <bits/stdc++.h>

using namespace std;

// long long dp[100];

long long find_path(long long n){
    if (n == 0){
        return 1;
    }
    if (n < 0){
        return 0;
    }
    long long sum = 0;
    for (int i = 1; i <= 6; i++){
        sum += find_path(n - i);
    }
    return sum;
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    for (int i = 0; i < 100; i++) dp[i] = 0;
    int n;
    cin >> n;

    dp[0] = 1;

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= 6; j++){
            if (i - j >= 0){

                dp[i] += dp[i - j];
            }
        }
    }

    cout << dp[n] << endl;

    // cout << find_path(n);

}
