#include <bits/stdc++.h>

using namespace std;

void findMaxAndMin(int array[5]){
    int maxInput = -1e9;
    int minInput = 1e9;
    for (int i = 0; i < 5; i++){
        if (array[i] > maxInput){
            maxInput = array[i];
        }
        if (array[i] < minInput){
            minInput = array[i];
        }
    }
    cout << minInput << endl;
    cout << maxInput << endl;
}

int main(){
    int arrayInput[5] = {};
    for (int i = 0; i < 5; i++){
        cin >> arrayInput[i];
    }
    findMaxAndMin(arrayInput);
}