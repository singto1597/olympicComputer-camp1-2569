#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int n;
    cin >> n;

    long long height_of_log[n + 5] = {};

    for (int i = 0; i < n; i++) cin >> height_of_log[i];

    long long max_volume = 0;

    int max_pos_i, max_pos_j;


    for (int i = 0; i < n - 1; i++){
        for (int j = i + 1; j < n; j++){
            long long volume = abs(min(height_of_log[i], height_of_log[j]) * (i - j));
            // cout << volume << endl;
            if (volume > max_volume){
                max_volume = volume;
                max_pos_i = i;
                max_pos_j = j;
            }
        }
    }
    cout << max_pos_i << " " << max_pos_j << endl;
    cout << max_volume << endl;
}
