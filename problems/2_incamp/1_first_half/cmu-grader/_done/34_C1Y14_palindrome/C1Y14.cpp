#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

int main() {
    string stadium;
    cin >> stadium;
    string stadiumReverse = stadium;
    reverse(stadiumReverse.begin(), stadiumReverse.end());
    if (stadium == stadiumReverse) {
        cout << stadium << " is a palindrome" << endl;
    } else {
        cout << stadium << " is not a palindrome" << endl;
    }

    return 0;
}