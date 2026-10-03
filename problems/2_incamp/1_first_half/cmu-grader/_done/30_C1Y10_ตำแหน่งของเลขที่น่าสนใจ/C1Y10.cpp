#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n;
    cin >> n;
    int number[n] = {};
    for (int i = 0; i < n; i++){
        cin >> number[i];
    }
    int needNumber;
    cin >> needNumber;
    int count = 0;
    for (int i = 1; i <= n; i++){
        if (number[i - 1] == needNumber) {
            cout << i << " "; 
            count++;
        }
        
    }
    if (count == 0) cout << "0";
    return 0;
    
}