#include <bits/stdc++.h>

using namespace std;
char table[50][50];
char table_next[50][50];
char table_bomb[50][50];

int main(){
    int n, m;

    cin >> n >> m;


    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            table[i][j] = '#';
            table_next[i][j] = '#';
            table_bomb[i][j] = '#';
        }
    }

    int current_n, current_m;

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            cin >> table[i][j];
            if (table[i][j] == 'P'){
                current_n = i;
                current_m = j;
            }
        }
    }

    int maxBombs, commands_amount;

    cin >> maxBombs >> commands_amount;

    int bombs_amount = 0;

    bool isAlive = true;
    int bridge_broke = 0;

    int next_n;
    int next_m;


    // . คือพื้นที่ว่าง
    // # คือกำแพง ที่ ทำลายไม่ได้
    // + คือกำแพง ที่ ทำลายได้
    // P คือจุดเริ่มต้นของผู้เล่น

    // U ^
    // D v
    // L <
    // R >

    // B วางระเบิด
    // T ระเบิดทั้งหมด

    int dn[4] = {-1,  1,  0,  0};
    int dm[4] = { 0,  0, -1,  1};

    // 0 ^
    // 1 v
    // 2 <
    // 3 >

    for (int _ = 0; _ < commands_amount; _++){
        char command;

        cin >> command;

        if (!isAlive) continue;

        switch (command){
            //-------------------------------------
            case 'U':
                if (table[current_n - 1][current_m] == '.' && table_bomb[current_n - 1][current_m] != 'B') {
                    table[current_n][current_m] = '.';
                    current_n -= 1;
                    table[current_n][current_m] = 'P';
                }
                break;
            //-------------------------------------

            //-------------------------------------
            case 'D':
                if (table[current_n + 1][current_m] == '.' && table_bomb[current_n + 1][current_m] != 'B') {
                    table[current_n][current_m] = '.';
                    current_n += 1;
                    table[current_n][current_m] = 'P';
                }
                break;
            //-------------------------------------

            //-------------------------------------
            case 'L':
                if (table[current_n][current_m - 1] == '.' && table_bomb[current_n][current_m - 1] != 'B') {
                    table[current_n][current_m] = '.';
                    current_m -= 1;
                    table[current_n][current_m] = 'P';
                }
                break;
            //-------------------------------------

            //-------------------------------------
            case 'R':
                if (table[current_n][current_m + 1] == '.' && table_bomb[current_n][current_m + 1] != 'B') {
                    table[current_n][current_m] = '.';
                    current_m += 1;
                    table[current_n][current_m] = 'P';
                }
                break;
            //-------------------------------------

            //-------------------------------------
            case 'B':
                if (bombs_amount < maxBombs && table_bomb[current_n][current_m] != 'B'){
                    table_bomb[current_n][current_m] = 'B';
                    bombs_amount++;
                }

                break;
            //-------------------------------------

            //-------------------------------------
            case 'T':
                for (int i = 1; i <= n; i++){
                    for (int j = 1; j <= m; j++){
                        table_next[i][j] = table[i][j];
                    }
                }
                for (int i = 1; i <= n; i++){
                    for (int j = 1; j <= m; j++){
                        if (table_bomb[i][j] == 'B'){
                            for (int k = 0; k < 4; k ++){
                                next_n = i;
                                next_m = j;
                                while (next_n >= 1 && next_n <= n && next_m >= 1 && next_m <= m && table[next_n][next_m] != '#'){

                                    if (table[next_n][next_m] == '+') {
                                        if (table_next[next_n][next_m] != '.') {
                                            bridge_broke++;
                                            table_next[next_n][next_m] = '.';
                                        }
                                        break;
                                    }
                                    else if (table[next_n][next_m] == 'P'){
                                        table_next[next_n][next_m] = '.';
                                        isAlive = false;
                                        // break;
                                    }

                                    next_n = dn[k] + next_n;
                                    next_m = dm[k] + next_m;
                                }

                            }
                        }
                    }
                }
                for (int i = 1; i <= n; i++){
                    for (int j = 1; j <= m; j++){
                        table[i][j] = table_next[i][j];
                    }
                }
                bombs_amount = 0;
                for (int i = 1; i <= n; i++){
                    for (int j = 1; j <= m; j++){
                        table_bomb[i][j] = '#';
                    }
                }
                break;
            //-------------------------------------


        }
        // for (int i = 1; i <= n; i++){
        //     for (int j = 1; j <= m; j++){
        //         cout << table[i][j];
        //     }
        //     cout << endl;
        // }

    }


    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            if (isAlive && i == current_n && j == current_m) {
                cout << 'P';
            } else if (table_bomb[i][j] == 'B') {
                cout << 'o';
            } else {

                if(table[i][j] == 'P') cout << '.';
                else cout << table[i][j];
            }
        }
        cout << endl;
    }
    if (isAlive) cout << "Alive ";
    else cout << "Dead ";
    cout << bridge_broke << endl;

}
