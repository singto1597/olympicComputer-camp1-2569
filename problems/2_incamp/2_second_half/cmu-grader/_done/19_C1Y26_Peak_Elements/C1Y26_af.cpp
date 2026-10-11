#include <bits/stdc++.h>
using namespace std;

int main (){
    int numberOfList;
    cin >> numberOfList;
    int array[numberOfList+5] = {};
    for (int i = 0; i < numberOfList; i ++){
        cin >> array[i];
    }
    if (numberOfList == 1){
        cout << array[0] << " ";
        return 0;
    }
    for (int i = 0; i < numberOfList; i ++){
        if (i == 0){
            if (array[i] >= array[i + 1]){
                cout << array[i] << " " ;
            }
            
        }
        else if (i == numberOfList - 1){
            if (array[i] >= array[i - 1]){
                cout << array[i] << " " ;
            }
        }
        else{
            if (array [i] >= array[i - 1] && array[i] >= array[i + 1] ){
                cout << array[i] << " ";
            }
        }
    

    }

    // int a[3][4] = {
    //     { 1 , 0 , 12 , -1},
    //     { 7 ,-3 ,  2 ,  5},
    //     {-5 ,-2 ,  2,  -9}
    // };
    
    // for (int i = 0; i < 4; i++){
    //     for (int round = 0; round < 3; round++){
    //         for (int j = 0; j < 3 - 1; j++){
    //             if (a[j][i] > a[j + 1][i]){
    //                 int first = a[j][i];
    //                 int second = a[j + 1][i];
    //                 a[j][i] = second;
    //                 a[j + 1][i] = first;
    //             }
    //         }

    //     }
    // }
    // for (int i = 0; i < 3; i++){
    //     for (int j = 0; j < 4; j++){
    //         cout << a[i][j] << " ";
    //     }
    //     cout << endl;
    // }

}