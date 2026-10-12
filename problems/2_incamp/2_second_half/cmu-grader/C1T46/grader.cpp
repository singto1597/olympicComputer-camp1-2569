#include "C1T46.h"
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    while (n--) {
        string op;
        cin >> op;
        if (op == "push") {
            int value;
            cin >> value;
            push(value);
        } else if (op == "pop") {
            pop();
        } else if (op == "peek") {
            cout << peek() << endl;
        } else if (op == "size") {
            cout << size() << endl;
        } else if (op == "display") {
            display();
        }
    }

    return 0;
}