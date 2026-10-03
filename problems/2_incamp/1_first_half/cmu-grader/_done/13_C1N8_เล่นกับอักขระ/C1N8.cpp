#include <iostream>
using namespace std;
int main() {
    char input;
    cin >> input;
    int asciiOfInput = (int)input;
    if (asciiOfInput >= 48 && asciiOfInput <= 57) cout << "Number" << endl;
    else if ((asciiOfInput >= 65 && asciiOfInput <= 90) || (asciiOfInput >= 97 && asciiOfInput <= 122)) cout << "Character" << endl;
    else cout << "Special Characters" << endl;

    
    return 0;
    
}