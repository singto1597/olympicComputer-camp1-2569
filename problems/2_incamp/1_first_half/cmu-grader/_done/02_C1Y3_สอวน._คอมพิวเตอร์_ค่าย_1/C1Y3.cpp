#include <iostream>
#include <iomanip>
using namespace std;
int main() {
    long long A, B, C;
    long long D, E, F;
    cin >> A >> B >> C;
    cin >> D >> E >> F;
    int x = ((A % 10) * 100) + ((B % 10) * 10) + (C % 10);
    int y = ((D % 10) * 100) + ((E % 10) * 10) + (F % 10);
    int password = (x + y) % 1000;
    printf ("%03d\n", password);
}