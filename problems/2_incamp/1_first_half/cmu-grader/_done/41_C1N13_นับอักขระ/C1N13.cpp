#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cin >> s;

    bool hasUpper = false; // มีตัวใหญ่ไหม
    bool hasLower = false; // มีตัวเล็กไหม
    bool hasDigit = false; // มีตัวเลขไหม

    for (char c : s) {
        if (isupper(c)) hasUpper = true;
        else if (islower(c)) hasLower = true;
        else if (isdigit(c)) hasDigit = true;
    }
    cout << "[" << s.length() << "]";
    if (hasUpper && !hasLower && !hasDigit)
        cout << "All Capital Letter";
    else if (!hasUpper && (hasLower || hasDigit))
        cout << "All Small Letter";
    else
        cout << "Mix";

    return 0;
}
