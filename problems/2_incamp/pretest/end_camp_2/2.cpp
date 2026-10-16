#include <bits/stdc++.h>

using namespace std;


int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int T;  
    cin >> T;

    while (T--){

        long long n, k, m;
        cin >> n >> k >> m;

        long long power = 0;

        long long plus_all = 0;
        long long mult_all = 1;

        vector<long long> plus_vec;

        for (int i = 0; i < n; i++){
            char type;
            cin >> type;
            long long x;
            cin >> x;
            if (type == 'p'){
                // plus_all += x;
                plus_vec.push_back(x);
            }
            else if (type == 'm'){
                mult_all *= x;
            }
        }
        sort(plus_vec.begin(), plus_vec.end(), greater<long long>());
        for (int i = 0; i < k; i++){
            plus_all += plus_vec[i];
        }
        power = plus_all * mult_all;
        if (power >= m){
            cout << "Yes" << endl;
        }
        else{
            cout << "No" << endl;
        }

        
    }


}