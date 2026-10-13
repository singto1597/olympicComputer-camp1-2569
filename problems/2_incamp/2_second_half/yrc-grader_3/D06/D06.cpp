#include <bits/stdc++.h>

using namespace std;


int main(){
    int n;
    cin >> n;
    vector<long long> h;
    for (int i = 0; i < n; i++){
        long long a;
        cin >> a;
        h.push_back(a);
    }
    stack<long long> st;
    long long max_height = 0;
    long long last_height_idx = 0;

    st.push(0);
    for (int i = 1; i <= n; i++, last_height_idx++){

        while (!st.empty() && h[st.top()] > h[i]){
            long long check = st.top();
            st.pop();
            long long area = 0;
            if (!st.empty()){
                area = (i - st.top() - 1) * h[check];
            }
            else{
                area = i * h[check];
            }
            if (area > max_height){
                max_height = area;
            }
        }
        st.push(i);
    }


    cout << max_height << endl;
}
