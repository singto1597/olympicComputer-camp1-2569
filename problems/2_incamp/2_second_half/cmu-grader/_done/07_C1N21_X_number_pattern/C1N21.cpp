#include <bits/stdc++.h>
using namespace std;

int main (){
    int n;
    cin >> n;

    for (int i = 0; i < n ; i++){
        for (int j = 0; j < n * 2 - 1; j++){
            if (i == j || i == 2 * n - 2 - j){
                cout << (i + 1);

            }
            else{
                cout << " ";
            }
        }
        cout << endl;
    }
    for (int i = n - 2; i >= 0 ; i--){
        for (int j = 0; j < n * 2 - 1; j++){
            if (i == j || i == 2 * n - 2 - j){
                cout << (i + 1);

            }
            else{
                cout << " ";
            }
        }
        cout << endl;
    }

}