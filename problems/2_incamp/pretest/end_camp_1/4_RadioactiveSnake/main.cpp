#include <bits/stdc++.h>

using namespace std;

int main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int q, MOD;
    cin >> q >> MOD;

    deque<long long> dq;
    long long weight = 0;
    long long radioactive = 1;

    int zero_count = 0;

    while(q--){
        string cmd;
        cin >> cmd;
        long long val;

        if (cmd == "ingest"){
            cin >> val;
            dq.push_back(val);
            weight = (weight + val) % MOD;
            if (val != 0) {
                radioactive = (radioactive * val) % MOD;

            }
            else{
                zero_count++;
            }
        }
        else if(cmd == "digest"){
            if (dq.empty()){
                cout << "Nothing to digest." << endl;
                continue;
            }
            weight = (weight - dq.front()) % MOD;
            if (dq.front() != 0){
                radioactive = (radioactive / dq.front());

            }
            else{
                zero_count--;
            }
            dq.pop_front();

        }
        else if(cmd == "weight"){
            cout << ((weight % MOD) + MOD) % MOD << endl;
        }
        else if(cmd == "radioactive"){
            if (zero_count == 0){
                cout << ((radioactive % MOD) + MOD) % MOD << endl;

            }
            else {
                cout << 0 << endl;
            }

        }
    }
}