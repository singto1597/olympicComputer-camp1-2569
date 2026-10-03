#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<long long> input = {};
    for (int i = 0; i < 3; i++){
        long long number;
        cin >> number;
        input.push_back(number);
    }
    sort(input.begin(), input.end());
    for (int i = 2; i >= 0; i--){
        cout << input[i] << " ";
    }
    
    return 0;
    
}