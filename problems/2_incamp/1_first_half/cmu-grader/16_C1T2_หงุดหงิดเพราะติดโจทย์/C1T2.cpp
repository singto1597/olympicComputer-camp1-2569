#include <iostream>
using namespace std;
int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    long long n;
    cin >> n;

    long long min_number = 100000, max_number = -1;

    for (int i = 0; i < n; i++){
        long long temp;
        cin >> temp;
        if (temp < min_number) min_number = temp;
        if (temp > max_number) max_number = temp;
    }

    int diff = max_number - min_number;

    long long m, max_diff;

    cin >> m >> max_diff;

    for (int i = 0; i < m; i++){
        long long v;
        cin >> v;
        
    }




    return 0;

}
