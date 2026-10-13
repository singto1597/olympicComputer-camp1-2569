#include "to_base.h"
#include <bits/stdc++.h>

using namespace std;

string toBase(long long n, int b) {
    char c[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
    string s;
    if (n == 0){
        return "0";
    }
    while (n > 0){
        long long a = n % b;
        if (a < 10){
            s += to_string(a);
        }
        else{
            s += c[a % 10];
        }
        n /= b;
    }
    reverse(s.begin(), s.end());
    return s;
}
