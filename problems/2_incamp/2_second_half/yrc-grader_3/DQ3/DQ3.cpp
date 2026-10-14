#include <bits/stdc++.h>

using namespace std;


int main(){
    int q;
    cin >> q;
    deque<int> dq;
    char pos = 'F';
    while (q--){
        string cmd;
        cin >> cmd;
        int x;
        if (cmd == "PF"){
            cin >> x;
            if (pos == 'F'){
                dq.push_front(x);
            }
            else{
                dq.push_back(x);
            }

        }


        else if (cmd == "PB"){
            cin >> x;
            if (pos == 'F'){
                dq.push_back(x);
            }
            else{
                dq.push_front(x);
            }

        }
        else if (cmd == "OF"){
            if (dq.empty()) continue;
            if (pos == 'F'){
                dq.pop_front();
            }
            else{
                dq.pop_back();
            }

        }
        else if (cmd == "OB"){
            if (dq.empty()) continue;
            if (pos == 'F'){
                dq.pop_back();
            }
            else{
                dq.pop_front();
            }

        }
        else if (cmd == "REV"){
            if      (pos == 'F') pos = 'B';
            else if (pos == 'B')   pos = 'F';

        }
        else if (cmd == "FRONT"){
            if (dq.empty()){
                cout << "EMPTY" << endl;
                continue;
            }
            if (pos == 'F'){
                cout << dq.front() << endl;
            }
            else{
                cout << dq.back() << endl;
            }

        }
        else if (cmd == "BACK"){

            if (dq.empty()){
                cout << "EMPTY" << endl;
                continue;
            }
            if (pos == 'F'){
                cout << dq.back() << endl;
            }
            else{
                cout << dq.front() << endl;
            }

        }


    }
    return 0;
}
