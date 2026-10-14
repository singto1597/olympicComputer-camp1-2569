#include <bits/stdc++.h>

using namespace std;


int main(){
    int n;
    cin >> n;

    queue<int> q;
    queue<int> index;

    int t;
    cin >> t;

    for (int i = 0; i < n; i++){
        int x;
        cin >> x;
        q.push(x);
        index.push(i + 1);
    }
    while (!q.empty()){
        if (q.front() <= t){
            cout << index.front() << " ";
            q.pop();
            index.pop();
        }
        else{
            int curr_value = q.front();
            int curr_index = index.front();
            q.pop();
            index.pop();
            curr_value -= t;
            q.push(curr_value);
            index.push(curr_index);
        }
    }
    return 0;
}
