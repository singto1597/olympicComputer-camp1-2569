#include <iostream>
#include <iomanip>
using namespace std;
int main() {
    int sum = 0;
    int input;
    double avg;
    for (int i = 0; i < 5; i++){
        cin >> input ;
        sum+=input;
    }
    avg = (double)sum / 5;
    cout << sum << endl;
    printf("%.2f", avg);
}