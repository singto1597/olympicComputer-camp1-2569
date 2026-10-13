#include <bits/stdc++.h>

using namespace std;

/*
f(-) = 0
f(0) = 1
f(1) = 1

f(n) =
    f(n - 1) + f(n - 2) + f(n - 3) +
    f(n - 4) + f(n - 5) + f(n - 6)

f(n - 1) =
    f(n - 2) + f(n - 3) + f(n - 4) +
    f(n - 5) + f(n - 6) + f(n - 7)

f(n) - f(n - 1) =
    f(n - 1) +
    f(n - 2) - f(n - 2) +
    f(n - 3) - f(n - 3) +
    f(n - 4) - f(n - 4) +
    f(n - 5) - f(n - 5) +
    f(n - 6) - f(n - 6) +
    f(n - 7)

f(n) - f(n - 1) =
    f(n - 1) +
    f(n - 7)

f(n) - f(n - 1) =
    f(n - 1) - f(n - 7)

f(n) =
    2f(n - 1) - f(n - 7)
*/

long long find_path(long long n){
    if (n == 0 || n == 1){
        return 1;
    }
    if (n < 0){
        return 0;
    }

    return 2LL * find_path(n - 1) - find_path(n - 7);

}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int n;
    cin >> n;
    cout << find_path(n) << endl;


}
