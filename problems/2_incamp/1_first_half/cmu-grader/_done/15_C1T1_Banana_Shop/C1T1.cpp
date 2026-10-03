#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
string sumeryStringFunction(string firstNumber, string secondNumber);
string differenceStringFunction(string firstNumber, string secondNumber);
int main() {
    // 

    string bananaRujHave;
    string bananaBuyFromJJ;
    string bananaKaoNeedToBuy;

    cin >> bananaRujHave;
    cin >> bananaBuyFromJJ;
    cin >> bananaKaoNeedToBuy;

    string bananaRujHaveAfterFromJJ = sumeryStringFunction(bananaRujHave, bananaBuyFromJJ);
    string bananaAfterKaoBut = differenceStringFunction(bananaRujHaveAfterFromJJ, bananaKaoNeedToBuy);
    cout << bananaAfterKaoBut << endl;
    
    return 0;
    
}

string sumeryStringFunction(string firstNumber, string secondNumber){
    string sumeryNumber = "";
    bool isOverTen = false;
    int digitSum = 0;
    while (secondNumber.length() < firstNumber.length()) secondNumber = "0" + secondNumber;
    while (secondNumber.length() > firstNumber.length()) firstNumber = "0" + firstNumber;
    for (int digitIndex = firstNumber.length() - 1; digitIndex >= 0; digitIndex--){
        digitSum = (firstNumber[digitIndex] - '0') + (secondNumber[digitIndex] - '0');
        if (isOverTen) digitSum += 1;
        isOverTen = false;
        if (digitSum / 10 >= 1) isOverTen = true;
        digitSum = digitSum % 10;
        sumeryNumber = to_string(digitSum) + sumeryNumber;
    }
    if (isOverTen) sumeryNumber = '1' + sumeryNumber;
    return sumeryNumber;
}

string differenceStringFunction(string firstNumber, string secondNumber) {
    string differenceNumber = "";
    bool isBorrowNextDigits = false;

    while (secondNumber.length() < firstNumber.length()) secondNumber = "0" + secondNumber;
    while (secondNumber.length() > firstNumber.length()) firstNumber = "0" + firstNumber;

    bool isNegative = false;
    if (secondNumber > firstNumber) {
        swap(firstNumber, secondNumber);
        isNegative = true;
    }

    for (int digitIndex = firstNumber.length() - 1; digitIndex >= 0; digitIndex--) {
        int digitA = firstNumber[digitIndex] - '0';
        int digitB = secondNumber[digitIndex] - '0';

        if (isBorrowNextDigits) {
            digitA -= 1;
            isBorrowNextDigits = false;
        }

        int digitsMinus = digitA - digitB;

        if (digitsMinus < 0) {
            digitsMinus += 10;
            isBorrowNextDigits = true;
        }

        differenceNumber = to_string(digitsMinus) + differenceNumber;
    }

    while (differenceNumber.size() > 1 && differenceNumber[0] == '0') {
        differenceNumber.erase(0, 1);
    }

    if (isNegative) differenceNumber = "-" + differenceNumber;

    return differenceNumber;
}