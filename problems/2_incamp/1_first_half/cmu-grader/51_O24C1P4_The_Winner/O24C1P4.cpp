#include <iostream>
#include <iomanip>
using namespace std;

long long power(long long base, long long exponent, long long mod){
    long long result = 1;
    base %= mod;
    while (exponent > 0) {
        if (exponent % 2 == 1)  // ถ้า exp เป็นเลขคี่
            result = (result * base) % mod;
        base = (base * base) % mod;
        exponent /= 2;
    }
    return result;
    
}

int main() {
    long long a, b, k;
    cin >> a >> b >> k;
    long long mod = 1;
    for (int i = 0; i < k; i++) mod *= 10;
    long long APowerB = power(a,b,mod);
    cout << setw(k) << setfill('0') << APowerB << endl;
    
    
    return 0;
    
}