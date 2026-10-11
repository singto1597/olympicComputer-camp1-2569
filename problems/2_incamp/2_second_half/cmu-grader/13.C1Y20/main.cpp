#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    if (n < 1 || n > 100){
        cout << "Error" << endl;
        return 0;
    }
    int array[n] = {};
    for (int i = 0; i < n; i++){
        cin >> array[i];
        if (array[i] < -100 || array[i] > 100){
            cout << "Error" << endl;
            return 0;
        }
    }

    int *poiterOfArray = &array[0];
    int sumOfNumber = 0;
    int index = 0;


    while (index < n){
        sumOfNumber += *poiterOfArray;
        index++;
        poiterOfArray++;
    }
    cout << sumOfNumber << endl;
}