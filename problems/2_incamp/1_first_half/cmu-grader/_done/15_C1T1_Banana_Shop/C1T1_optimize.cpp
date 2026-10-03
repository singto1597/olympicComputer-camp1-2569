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

string sumeryStringFunction(string firstNumber, string secondNumber) {
    // ทำให้ยาวเท่ากัน
    if (firstNumber.size() < secondNumber.size())
        firstNumber.insert(0, secondNumber.size() - firstNumber.size(), '0');
    else if (secondNumber.size() < firstNumber.size())
        secondNumber.insert(0, firstNumber.size() - secondNumber.size(), '0');

    string result;
    result.reserve(firstNumber.size() + 1); // จองพื้นที่ล่วงหน้า
    int carry = 0;

    // บวกจากหลังไปหน้า
    for (int i = firstNumber.size() - 1; i >= 0; --i) {
        int sum = (firstNumber[i] - '0') + (secondNumber[i] - '0') + carry;
        result.push_back((sum % 10) + '0');
        carry = sum / 10;
    }

    if (carry) result.push_back('1');

    reverse(result.begin(), result.end());
    return result;
}

string differenceStringFunction(string firstNumber, string secondNumber) {
    // ทำให้ยาวเท่ากัน
    if (firstNumber.size() < secondNumber.size())
        firstNumber.insert(0, secondNumber.size() - firstNumber.size(), '0');
    else if (secondNumber.size() < firstNumber.size())
        secondNumber.insert(0, firstNumber.size() - secondNumber.size(), '0');

    bool isNegative = false;
    if (secondNumber > firstNumber) {
        swap(firstNumber, secondNumber);
        isNegative = true;
    }

    string result;
    result.reserve(firstNumber.size());
    int borrow = 0;

    for (int i = firstNumber.size() - 1; i >= 0; --i) {
        int a = (firstNumber[i] - '0') - borrow;
        int b = secondNumber[i] - '0';

        if (a < b) {
            a += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.push_back((a - b) + '0');
    }

    while (result.size() > 1 && result.back() == '0') result.pop_back();

    if (isNegative) result.push_back('-');

    reverse(result.begin(), result.end());
    return result;
}
