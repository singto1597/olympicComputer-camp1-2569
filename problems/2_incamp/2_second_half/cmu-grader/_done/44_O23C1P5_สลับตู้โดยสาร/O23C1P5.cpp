#include <bits/stdc++.h>
using namespace std;

int main (){
    int n;
    cin >> n;
    int sumOfSwap = 0;
    for (int _ = 0; _ < n; _++){
        int numberOfTrain;
        cin >> numberOfTrain;
        int array[numberOfTrain];
        for (int i = 0; i < numberOfTrain; i++){
            cin >> array[i];
        }

        int count = 0;
        for (int i = 0; i < numberOfTrain; i++){
            for (int j = 0; j < numberOfTrain - i - 1; j ++){
                if (array[j] > array[j + 1]){
                    swap (array[j], array[j + 1]);
                    count ++;
                }
            }
        }
        sumOfSwap += count;
        cout << count << endl;
    }
    cout << sumOfSwap << endl;

}