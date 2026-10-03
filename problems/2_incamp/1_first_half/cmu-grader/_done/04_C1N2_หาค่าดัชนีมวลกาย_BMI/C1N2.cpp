#include <iostream>
using namespace std;
int main() {
    int weight = 0;
    double high = 0.00;
    cin >> weight >> high;
    double bmi = weight / (high * high);
    printf ("%.2f\n", (double)weight);
    printf ("%.2f\n", high);
    printf ("%.2f\n", bmi);
}                       