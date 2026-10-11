#include <bits/stdc++.h>

using namespace std;

int main(){
    int n = 2;
    int array[n] = {};
    for (int i = 0; i < n; i++){
        cin >> array[i];
        if (array[i] < -100 || array[i] > 100){
            cout << "Error" << endl;
            return 0;
        }
    }

    int *poiterOfArray = &array[0];
    cout << *(poiterOfArray + 1) << " " << *poiterOfArray << endl;
}