#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    int n;
    cin >> n;

    if (n < 3 || n > 100) {
        cout << "Invalid\n";
        return 0;
    }

    vector<int> array(n);
    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }

    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (array[j] < array[min_index]) {
                min_index = j;
            }
        }
        int temp = array[i];
        array[i] = array[min_index];
        array[min_index] = temp;
    }

    int minNumber = array[0];
    int maxNumber = array[n - 1];

    double median;
    if (n % 2 == 0) {
        median = (array[n / 2] + array[(n / 2) - 1]) / 2.0;
    } else {
        median = array[n / 2];
    }

    cout << minNumber << "\n";
    cout << maxNumber << "\n";

    if (median == (int)median) {
        cout << (int)median << "\n";
    } else {
        cout << fixed << setprecision(2) << median << "\n";
    }

    return 0;
}
