#include <bits/stdc++.h>
using namespace std;

struct Command {
    char type;
    int value;
};

struct LoopState {
    int returnIndex;
    int loopLeft;
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int n;
    cin >> n;

    vector<Command> programs;

    stack<LoopState> loop_state;

    for (int i = 0; i < n; i++){
        char command;
        int value = -1;
        if (command == 'E'){
            cin >> command;
        }
        else{
            cin >> command >> value;

        }
        programs.push_back({command, value});
    }

    int program_counter = 0;

    while (program_counter < (int)programs.size()){
        Command cmd = programs[program_counter];



        if (cmd.type == 'L'){
            loop_state.push({program_counter + 1, cmd.value});
            program_counter++;
            continue;
        }

        if (cmd.type == 'E'){

            if (!loop_state.empty()){
                LoopState current = loop_state.top();
                // cout << program_counter << endl;
                // cout << current.returnIndex << " " << current.loopLeft << endl;
                if (current.loopLeft <= 1){
                    loop_state.pop();
                    program_counter++;
                }
                else {
                    loop_state.pop();
                    loop_state.push({current.returnIndex, current.loopLeft - 1});
                    program_counter = current.returnIndex;
                }
            }

            else {
                program_counter++;
            }

            continue;

        }

        program_counter++;

        cout << cmd.type << " " << cmd.value << endl;

    }



    return 0;

}

