#include <bits/stdc++.h>

using namespace std;


int main(){
    stack<int> st;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++){
        int x;
        cin >> x;
        if (x == 0){
            if (!st.empty()){
                st.pop();
            }
        }
        else{
            st.push(x);

        }
    }
    if (st.empty()){
        cout << "EMPTY";
        return 0;
    }
    while(!st.empty()){
        cout << st.top() << " ";
        st.pop();
    }
    return 0;
}
