#include <iostream>
using namespace std;

int main() {
    int input = 0;
    int maxNumber = 0;
    int minNumber = 10000;
    double sum = 0;
    for (int i = 0; i < 10; i++){
        cin >> input;
        if (input > maxNumber){
            maxNumber = input;
        }
        if (input < minNumber){
            minNumber = input;
        }
        sum += input;

    }
    cout << minNumber << endl;
    cout << maxNumber << endl;
    printf ("%.2f\n",sum/10);

    return 0;
}