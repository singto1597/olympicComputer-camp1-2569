#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, m;
    cin >> n >> m;

    vector<int> home(n);
    vector<int> heater(m);

    
    for (int i = 0; i < n; i++){
        cin >> home[i];
    }
    for (int i = 0; i < m; i++){
        cin >> heater[i];
    }
    
    sort(home.begin(), home.end());
    sort(heater.begin(), heater.end());

    // cout << abs(home[n - 1] - heater[0]) << endl;
    int max = 0;
    for (int i = 0; i < n; i++){
        if (home[i] - heater[i] > max){
            max = home[i] - heater[i];
        }
    }
    cout << max << endl;
}