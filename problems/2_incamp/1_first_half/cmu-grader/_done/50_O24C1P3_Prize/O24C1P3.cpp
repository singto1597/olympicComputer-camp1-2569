#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    long long n;
    cin >> n;
    string number;
    cin >> number;
    long long result = 1;
    for (int i = 1; i <= number.length(); i++){
        int digit = number[number.length() - i] - '0';
        if (n == 1){
            if (i % 2 == 1) result *= digit;
        }
        else if (n == 2){
            if (i % 2 == 0) result *= digit;
        }
        else{
            result *= digit;
        }
    }
    cout << result << endl;
    
    
    return 0;
    
}