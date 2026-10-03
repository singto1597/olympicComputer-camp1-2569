#include <iostream>
using namespace std;
int main() {
    int n;
    cin >> n;
    if (n >= 1000){
        cout << "100" << endl;
    }
    else if (n >= 0){
        cout << "0" << endl;
    }
    else {
        cout << "Error" << endl;
    }
}                       