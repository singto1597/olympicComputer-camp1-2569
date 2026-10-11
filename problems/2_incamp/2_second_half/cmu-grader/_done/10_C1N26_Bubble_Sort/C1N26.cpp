#include <bits/stdc++.h>

using namespace std;

int main(){
    set<int> myList = {};
    int n;
    cin >> n;
    for (int i = 0; i < n; i++){
        int buffer;
        cin >> buffer;
        myList.insert(buffer);
    }
    cout << myList.size() << endl;
    for (int x : myList){
        cout << x << " ";
    }
}