#include <bits/stdc++.h>

using namespace std;

int dx[8] = {-1, -1, -1,  0,  0,  1,  1,  1};

int dy[8] = {-1,  0,  1, -1,  1, -1,  0,  1};

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int m, n;
    cin >> m >> n;

    int round;
    cin >> round;

    vector<vector<int>> table(m, vector<int>(n, 0));
    vector<vector<int>> table_next(m, vector<int>(n, 0));

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            cin >> table[i][j];
        }
    }

    for (int _ = 0; _ < round; _++){
        for (int i = 0; i < m; i++){
            for (int j = 0; j < n; j++){
                int count = 0;
                for (int k = 0; k < 8; k++){
                    int x = j + dx[k];
                    int y = i + dy[k];
                    if (x >= 0 && y >= 0 && x < n && y < m){
                        count += table[y][x];
                    }
                }
                if (count < 2){
                    table_next[i][j] = 0;
                }
                else if (count == 2 || count == 3){
                    if (table[i][j]){
                        table_next[i][j] = 1;
                    }
                    else{
                        if (count == 3){
                            table_next[i][j] = 1;
                        }
                        else{
                            table_next[i][j] = 0;
                        }
                    }
                }
                else{
                    table_next[i][j] = 0;
                }

            }
        }

        for (int i = 0; i < m; i++){
            for (int j = 0; j < n; j++){
                table[i][j] = table_next[i][j];
            }
        }
    }

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            cout << table[i][j] << " ";
        }
        cout << endl;
    }

}
