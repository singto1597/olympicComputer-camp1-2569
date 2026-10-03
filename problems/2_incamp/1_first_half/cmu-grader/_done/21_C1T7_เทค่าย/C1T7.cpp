#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n;
    cin >> n;
    int minNumber = 100000;
    int maxNumber = 2;
    int numberInput;
    for (int i = 0; i < n; i++){
        cin >> numberInput;
        if (minNumber > numberInput){
            minNumber = numberInput;
        }
        if (maxNumber < numberInput){
            maxNumber = numberInput;
        }
    }
    for (int i = minNumber; i <= maxNumber; i++){
        if (i < 2) continue; // ข้ามเลขที่น้อยกว่า 2
        bool isPrime = true;
        
        for (int j = 2; j * j <= i; j++){
            if (i % j == 0){
                isPrime = false;
                break;
            }
        }
        if (isPrime){
            cout << i << " ";
        }
        
    }
    
    return 0;
    
}