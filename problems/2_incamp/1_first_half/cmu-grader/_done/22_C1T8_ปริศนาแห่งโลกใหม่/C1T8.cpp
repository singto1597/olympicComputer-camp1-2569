#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    int bouk = 0;
    if (n % 2 == 1){
        bouk+=1;
    }
    for (int i = 0; i < n + n/2; i++){
        for (int j = 0; j < n + n/2; j++){
            if (j + 1 > n + i || i - j + 1 > n){
                cout << " ";
            }
            else{
                cout << (min(i,j) + 1) % 10;

            }
        }
        for (int j = n + n/2 - 1 + bouk; j >= 0; j--){
            if (j + 1 > n + i || i - j + 1 > n){
                cout << " ";
            }
            else{
                cout << (min(i,j) + 1) % 10;
            }
        }
        cout << endl;
    }

    int i = n + n/2 - 1;

    if (n % 2 == 1){
        i+=1;
    }

    for (; i >= 0 ; i--){
        for (int j = 0; j < n + n/2 + bouk; j++){
            if (j + 1 > n + i || i - j + 1 > n){
                cout << " ";
            }
            else{
                cout << (min(i,j) + 1) % 10;

            }
        }
        for (int j = n + n/2 - 1; j >= 0; j--){
            if (j + 1 > n + i || i - j + 1 > n){
                cout << " ";
            }
            else{
                cout << (min(i,j) + 1) % 10;
            }
        }
        cout << endl;
    }



    return 0;

}
