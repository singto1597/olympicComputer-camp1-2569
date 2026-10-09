#include <bits/stdc++.h>

using namespace std;

#define ll long long

int gcd(int a,int b){
    if(b == 0)return a;
    return gcd(b,a%b);
}

int main () {
    ll a,b;cin >> a >> b;
    while(b != 0){
        ll temp = a % b;
        a = b;
        b = temp;
    }
    cout << a;
}

/*

35 6
35 = 6 * 5 + 5
6 = 5 * 1 + 1
5 = 1 * 5 + 0
1 =

70 8
70 = 8 * 8 + 6
8 = 6 * 1 + 2
6 = 2 * 3 + 0

gcd = 2

*/
