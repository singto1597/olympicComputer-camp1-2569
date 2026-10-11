#include <bits/stdc++.h>
using namespace std;

int main (){
    int number[20];
    for (int i = 0; i < 10; i++){
        cin >> number[i];
    }
    int *poiter = &number[0];
    int sum = 0;
    for (int i = 0; i < 10; i++){
        if (*poiter % 2 == 0){
            sum += *poiter;
        }
        poiter ++;
    }
    cout << sum << endl;


}