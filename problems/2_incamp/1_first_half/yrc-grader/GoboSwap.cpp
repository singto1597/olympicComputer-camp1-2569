#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, round;

    cin >> n >> round;

    long long arr_org[n + 5] = {};

    for (int i = 0; i < n; i++) cin >> arr_org[i];

    for (int _ = 0; _ < round; _++){
        long long arr_1[n + 5] = {};
        long long arr_2[n + 5] = {};
        int x;
        cin >> x;

        for (int i = x; i < n; i ++){
            arr_2[i - x] = arr_org[i];
        }
        for (int i = 0; i < x; i ++){
            arr_1[i] = arr_org[i];
        }

        long long min_1 = 1e10, min_2 = 1e10;

        for (int i = 0; i < n - x; i++){
            if (arr_2[i] < min_2) min_2 = arr_2[i];
        }
        for (int i = 0; i < x; i++){
            if (arr_1[i] < min_1) min_1 = arr_1[i];
        }

        // cout << "1: " << min_1 << " 2: " << min_2 << endl;

        int idx = 0;

        if (min_2 < min_1){
            for (int i = 0; i < n - x; i++){
                cout << arr_2[i] << " ";
                arr_org[idx++] = arr_2[i];
            }
            for (int i = 0; i < x; i++){
                cout << arr_1[i] << " ";
                arr_org[idx++] = arr_1[i];
            }
        }
        else{
            for (int i = 0; i < x; i++){
                cout << arr_1[i] << " ";
                arr_org[idx++] = arr_1[i];
            }
            for (int i = 0; i < n - x; i++){
                cout << arr_2[i] << " ";
                arr_org[idx++] = arr_2[i];
            }

        }

        // cout << "\n";
        cout << "\n";
    }





}
