#include <iostream>
using namespace std;
int main() {
    int n;
    cin >> n;
    bool isCanDevidedBy_3 = n % 3 == 0;
    bool isCanDevidedBy_5 = n % 5 == 0;

    if (isCanDevidedBy_3 == true && isCanDevidedBy_5 == false) cout << "3" << endl;
    else if (isCanDevidedBy_3 == false && isCanDevidedBy_5 == true) cout << "5" << endl;
    else if (isCanDevidedBy_3 == true && isCanDevidedBy_5 == true) cout << "35" << endl;
    else if (isCanDevidedBy_3 == false && isCanDevidedBy_5 == false) cout << "5555" << endl;
    return 0;
    
}