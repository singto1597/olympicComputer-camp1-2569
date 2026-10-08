#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;

    for (int _ = 0; _ < n; _++){
        int k;
        cin >> k;

        int arr[k] = {};

        for (int i = 0; i < k - 1; i ++){
            cin >> arr[i];
        }

        if (k == 4){
            int diff1 = arr[1] - arr[0];
            int diff2 = arr[2] - arr[1];
            int diff;
            if (diff1 != diff2){
                if (abs(diff1) < abs(diff2)){
                    diff = diff1;
                    cout << arr[1] + diff;
                }
                else{
                    diff = diff2;
                    cout << arr[0] + diff;
                }
            }
            else{
                diff = diff1;
                cout << arr[2] + diff;
            }
        }
        else if (k == 3){
            int diff = arr[1] - arr[0];
            if (diff % 2 == 0){
                cout << (arr[0] + arr[1]) / 2;
            }
            else{
                cout << arr[1] + diff;
            }
        }
        else{
            int diff1 = arr[1] - arr[0];
            int diff2 = arr[2] - arr[1];

            int diff;
            if (diff1 != diff2){
                if (abs(diff1) < abs(diff2)){
                    diff = diff1;
                    cout << arr[1] + diff;
                }
                else{
                    diff = diff2;
                    cout << arr[0] + diff;
                }

            }
            else{
                diff = diff1;
                for (int i = 2; i < k - 1; i ++){
                    int curr_diff = arr[i + 1] - arr[i];
                    if (curr_diff != diff){
                        cout << arr[i] + diff;
                        break;
                    }

                    if (i == k - 1){
                        cout << arr[i] + diff;
                    }

                }
            }
        }
        cout << endl;
    }
}
