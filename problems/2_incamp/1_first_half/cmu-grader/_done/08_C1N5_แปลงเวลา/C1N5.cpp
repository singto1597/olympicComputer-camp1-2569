#include <iostream>
using namespace std;
int main() {
    int minutesAll;
    cin >> minutesAll;
    int days = minutesAll / (24 * 60);
    int hours = (minutesAll - (days * 24 * 60)) / 60;
    int minutes = minutesAll - (days * 24 * 60 + (hours * 60));
    cout << days << endl;
    cout << hours << endl;
    cout << minutes << endl;
}