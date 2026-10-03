#include <iostream>
using namespace std;
int main() {
    long long money;
    cin >> money;
    long long thousand = money / 1000;
    long long fiveHundred = (money - (thousand * 1000)) / 500;
    long long hundred = (money - (thousand * 1000 + fiveHundred * 500)) / 100;
    long long fifty = (money - (thousand * 1000 + fiveHundred * 500 + hundred * 100)) / 50;
    long long twenty = (money - (thousand * 1000 + fiveHundred * 500 + hundred * 100 + fifty * 50)) / 20;
    long long ten = (money - (thousand * 1000 + fiveHundred * 500 + hundred * 100 + fifty * 50 + twenty * 20)) / 10;
    long long five = (money - (thousand * 1000 + fiveHundred * 500 + hundred * 100 + fifty * 50 + twenty * 20 + ten * 10)) / 5;
    long long one = (money - (thousand * 1000 + fiveHundred * 500 + hundred * 100 + fifty * 50 + twenty * 20 + ten * 10 + five * 5));
    cout << thousand << endl;
    cout << fiveHundred << endl;
    cout << hundred << endl;
    cout << fifty << endl;
    cout << twenty << endl;
    cout << ten << endl;
    cout << five << endl;
    cout << one << endl;
}