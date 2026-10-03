#include <iostream>
using namespace std;
int main() {
    int n;
    int sumOfEven = 0;
    int sumOfOdd = 0;
    for (int i = 0; i < 8; i++){
        cin >> n;
        if (n % 2 == 0){
            sumOfEven += n;
        }
        else {
            sumOfOdd += n;
        }
    }
    if (sumOfEven > sumOfOdd){
        cout << "even" << endl;
    }
    else if (sumOfEven < sumOfOdd){
        cout << "odd" << endl;
    }
    else {
        cout << "equal" << endl;
    }
    cout << sumOfEven << endl;
    cout << sumOfOdd << endl;
    return 0;
    
}