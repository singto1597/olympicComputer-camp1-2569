#include <bits/stdc++.h>
using namespace std;

void move_left(auto &it, string &answer){
    if (it != answer.begin() && !answer.empty()){
        it--;
    }
}

void move_right(auto &it, string &answer){
    if (it != answer.end() && !answer.empty()){
        it++;
    }
}

void backspace(auto &it, string &answer){
    if (it != answer.begin()) {
        it--;
        answer.erase(it);
    }
}

void move_begin(auto &it, string &answer){
    it = answer.begin();
}

void move_end(auto &it, string &answer){
    it = answer.end();
}

int main() {
    string s;
    cin >> s;
    string answer = "";
    auto it = answer.begin();

    for (int i = 0; i < (int)s.size(); i++){
        char cur = s[i];

        if (cur != '<' && cur != '>' && cur != '-' && cur != '[' && cur != ']'){

            it = answer.insert(it, cur);
            ++it;
            continue;
        }
        switch (cur){
            case '<':
                // if (it != answer.begin() && !answer.empty()){
                //     it--;
                // }
                move_left(it, answer);
                break;

            case '>':
                // if (it != answer.end() && !answer.empty()){
                //     it++;
                // }
                move_right(it, answer);
                break;

            case '-':
                // if (it != answer.begin()) {
                //     it--;
                //     answer.erase(it);
                // }
                backspace(it, answer);
                break;

            case '[':
                // it = answer.begin();
                move_begin(it, answer);
                break;

            case ']':
                // it = answer.end();
                move_end(it, answer);
                break;
        }
    }

    cout << answer;

    return 0;

}

