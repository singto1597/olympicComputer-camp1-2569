#include <bits/stdc++.h>
using namespace std;

int main (){
    //A 50,000
    //B 105,000
    //C 250,000

    char typeOfCar;
    cin >> typeOfCar;

    int numberOfCar;
    cin >> numberOfCar;

    double allCommition = 0;

    switch (typeOfCar)
    {
    case 'A':
        allCommition = 50000 * numberOfCar;
        break;
    case 'B':
        allCommition = 105000 * numberOfCar;
        break;
    case 'C':
        allCommition = 250000 * numberOfCar;
        break;
    default:
        cout << "Invalid" << endl;
        return 0;
        break;
    }

    cout << fixed << setprecision(2) << allCommition << endl;

}