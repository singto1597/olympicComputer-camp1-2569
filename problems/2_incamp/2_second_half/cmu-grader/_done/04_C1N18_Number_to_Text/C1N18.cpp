#include <bits/stdc++.h>
using namespace std;

int main (){
    const string numberInMessage[10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    string number;
    cin >> number;
    for (int i = 0; i < number.length(); i++){
        cout << numberInMessage[number[i] - '0'] << " ";
    }

}