#include <bits/stdc++.h>

using namespace std;


char tables[100][100] = {};  // table
int sum_fire[100][100] = {}; // ค่า e ที่สะสม
int sum_time[100][100] = {}; // ค่า เวลา ที่ไหม้สะสม

int dr[4] = {-1,  0,  1,  0};
int dc[4] = { 0,  1,  0, -1};
int main(){
    char command;

    cin >> command;

    int r, c, d;

    cin >> r >> c >> d;

    bool is_fire_now = true;

    for (int i = 1; i <= r; i++){
        for (int j = 1; j <= c; j++){
            cin >> tables[i][j];
            // cout << tables[i][j];
        }
    }

    int time_to_dub = 0;
    int block_fired = 0;

    while (is_fire_now){
        is_fire_now = false;

        for (int i = 1; i <= r; i++){
            for (int j = 1; j <= c; j++){
                if (tables[i][j] == 'F'){
                    is_fire_now = true;
                    if (sum_time[i][j] <= d){
                        sum_time[i][j]++;

                    }
                    if (sum_time[i][j] > d){
                        sum_time[i][j]++;
                        block_fired++;
                        tables[i][j] = 'X';
                    }
                    for (int d = 0; d < 4; d++){
                        int m = i + dr[d];
                        int n = j + dc[d];
                        sum_fire[m][n]++;
                        if (sum_fire[m][n] >= tables[m][n] - '0'){
                            
                        }
                    }
                }



            }

        }


        time_to_dub++;
    }

    cout << time_to_dub + d << " " << "1" << endl;
    for (int i = 1; i <= r; i++){
        for (int j = 1; j <= c; j++){
            cout << tables[i][j];
        }
        cout << endl;
    }



}
