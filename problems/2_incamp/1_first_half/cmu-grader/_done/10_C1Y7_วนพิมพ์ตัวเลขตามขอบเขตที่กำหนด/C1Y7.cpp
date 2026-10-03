#include <iostream>
using namespace std;
int main() {
    long long start, end;
    cin >> start >> end;
    if (start > end){
        cout << "Invalid" << endl;
        return 0;
    }

    for (int i = start; i <= end; i++){
        cout << i << " ";
    }
    cout << endl;

    return 0;
    
}