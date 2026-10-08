#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int size;
    cin >> size;
    char c = 'A';
    int digit = 1;
    int count = 0;

    for (int i = 0; i < size * 2 + 1 + 2; i++) cout << "* ";
    cout << endl;
    for (int i = 0; i < size; i++){
        cout << "* ";
        for (int j = 0; j < i+1; j++){
            cout << ". ";
        }
        for (int j = 0; j < size - (i+1); j++){
            cout << "  ";
        }
        if (count == 0){
            cout << c << " ";
            c++;
            if(c > 'Z') {
                c = 'A';
            }
        }
        else{
            cout << digit << " ";
            digit++;
            if(digit > 9) {
                digit %= 9;
            }
        }
        count++;
        count %= 2;
        for (int j = 0; j < size - (i+1); j++){
            cout << "  ";
        }
        for (int j = 0; j < i+1; j++){
            cout << ". ";
        }
        cout << "* ";
        cout << endl;
    }
    for (int i = 0; i < size * 2 + 1 + 2; i++) cout << "* ";


}
