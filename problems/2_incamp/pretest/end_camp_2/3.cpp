#include <bits/stdc++.h>

using namespace std;

long long find_min(stack<long long> st){
    long long min = 1e18;
    while (!st.empty()){
        if (st.top() < min){
            min = st.top();
        }
        st.pop();
    }
    return min;
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    stack<long long> st;
    

    while(true){
        char cmd;
        cin >> cmd;
        long long k;
        
        if (cmd != 'A' && cmd != 'D' && cmd != 'Q'){
            break;
        }

        if (cmd == 'A'){
            cin >> k;
            st.push(k);
        }
        else if(cmd == 'D'){
            if (st.empty()){
                cout << "-1" << endl;
                continue;
            }

            cout << st.top() << endl;
            st.pop();

        }
        else if (cmd == 'Q'){
            if (st.empty()){
                cout << "-1" << endl;
                continue;
            }
            cout << find_min(st) << endl;
        }
        
    }


}