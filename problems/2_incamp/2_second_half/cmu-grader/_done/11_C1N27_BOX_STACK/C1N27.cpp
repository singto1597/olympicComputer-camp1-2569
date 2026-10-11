#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    int maxHightOfStack = 0;
    for (int i = 0; i < n; i++){
        int lengthOfBox, wideOfBox, highOfBox;
        cin >> lengthOfBox >> wideOfBox >> highOfBox;
        maxHightOfStack += max({lengthOfBox, wideOfBox, highOfBox});
    }
    cout << maxHightOfStack << endl;

}