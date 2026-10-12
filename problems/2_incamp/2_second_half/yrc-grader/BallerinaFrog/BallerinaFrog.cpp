#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    stack<int> brackets_open;
    int max_bracket = 0;
    int count = 0;
    bool isCorrect = false;

    string s;
    cin >> s;

    for (int i = 0; i < s.size(); i++){
        if (s[i] == '('){
            brackets_open.push(i);
        }
        else if (s[i] == ')'){
            if (!brackets_open.empty()){
                int index = brackets_open.top();
                brackets_open.pop();
                if (i - index > max_bracket){
                    max_bracket = i - index ;
                    // cout << index << " " << i << " " << count << endl;
                    count = 1;

                }
                else if (i - index == max_bracket){
                    count++;
                }
            }

        }
    }

    if (count != 0){
        cout << max_bracket + 1 << " " << count << endl;
    }
    else {
        cout << "0 1" << endl;
    }

    return 0;

}

