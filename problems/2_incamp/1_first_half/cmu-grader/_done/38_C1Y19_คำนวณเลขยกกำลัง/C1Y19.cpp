#include <iostream>
#include <cmath>
using namespace std;
long long powRecursive(long long m, long long n){
    if (n == 0){
        return 1;
    }
    else{
        return m * powRecursive(m, n-1);
    }
}
int main() {
    long long m, n;
    cin >> m >> n;
    long long powNumber = powRecursive(m,n);
    cout << powNumber << endl;

    return 0;
}