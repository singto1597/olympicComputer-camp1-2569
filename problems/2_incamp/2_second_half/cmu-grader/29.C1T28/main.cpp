#include <bits/stdc++.h>
using namespace std;

int main (){
    int sizeOfRactangle;
    cin >> sizeOfRactangle;
    char character1;
    char character2;
    char character3;
    char character4;
    cin >> character1 >> character2 >> character3 >> character4;
    for (int i = 0; i < sizeOfRactangle; i++){
        for (int j = 0; j < sizeOfRactangle - i; j++){
            cout << character1 <<" ";
        }
        for (int j = 0; j < i; j++){
            cout << character3 <<" ";
        }

        cout << "  ";
        
        for (int j = 0; j < i; j++){
            cout << character4 <<" ";
        }
        for (int j = 0; j < sizeOfRactangle - i; j++){
            cout << character2 <<" ";
        }
        cout << endl;
    }
    cout << endl;

    for (int i = 0; i < sizeOfRactangle; i++){
        for (int j = 0; j < i + 1; j++){
            cout << character2 <<" ";
        }
        for (int j = 0; j < sizeOfRactangle - i - 1; j++){
            cout << character4 <<" ";
        }

        cout << "  ";
        
        for (int j = 0; j < sizeOfRactangle - i - 1; j++){
            cout << character3 <<" ";
        }
        for (int j = 0; j < i + 1; j++){
            cout << character1 <<" ";
        }
        cout << endl;
    }
    cout << endl;

}