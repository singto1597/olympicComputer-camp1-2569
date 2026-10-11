#include <bits/stdc++.h>
using namespace std;

int main (){
    long long maxNumber = -1e18;
    long long minNumber = 1e18;
    long long numberInput;
    while (1){
        cin >> numberInput;
        if (numberInput == 0){
            break;
        }
        if (numberInput > maxNumber){
            maxNumber = numberInput;
        }
        if (numberInput < minNumber){
            minNumber = numberInput;
        }
    }
    if (maxNumber == -1e18 && minNumber == 1e18 && numberInput == 0){
        cout << "No data" << endl;
        return 0;
    }
    cout << maxNumber << " " << minNumber << endl;


}