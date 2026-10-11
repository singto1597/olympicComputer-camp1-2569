#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;

    int max_sum = 0;
    int max_base = 0;
    for (int base = 2; base <= 16; base++){
        int temp = n;
        int sum = 0;
        while (temp > 0){
            sum += temp % base;
            temp /= base;
        }
        if (sum > max_sum){
            max_sum = sum;
            max_base = base;
        }
    }
    cout << max_sum << " " << max_base << endl;
}
