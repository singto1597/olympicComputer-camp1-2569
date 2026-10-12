#include "C1T47.h"
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    while (n--) {
        string op;
        cin >> op;
        if (op == "enqueue") {
            int value;
            cin >> value;
            enqueue(value);
        } else if (op == "dequeue") {
            dequeue();
        } else if (op == "front") {
            cout << front() << endl;
        } else if (op == "rear") {
            cout << rear() << endl;
        } else if (op == "size") {
            cout << size() << endl;
        } else if (op == "display") {
            display();
        }
    }

    return 0;
}