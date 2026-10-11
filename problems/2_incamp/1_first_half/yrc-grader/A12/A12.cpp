#include <bits/stdc++.h>

using namespace std;

int main(){
    int m, n;
    cin >> m >> n;
    char table[m + 1][n + 1];
    for (int i = 0; i < m; i ++){
        for (int j = 0; j < n; j ++){
            cin >> table[i][j];
        }
    }

    int sum = 0;

    for (int i = 0; i < m; i ++){
        bool isS = false;
        for (int j = 0; j < n; j ++){
            if (table[i][j] == 'S') isS = true;
        }
        if (!isS) {
            sum += n;
            for (int j = 0; j < n; j ++){
                if (table[i][j] == '.') table[i][j] = 'o';
            }
        }
        // cout << isS << " " ;
    }
    // cout << endl;

    for (int i = 0; i < n; i ++){
        bool isS = false;
        for (int j = 0; j < m; j ++){
            if (table[j][i] == 'S') isS = true;
        }
        if (!isS){
            for (int j = 0; j < m; j ++){
                if (table[j][i] != 'o'){
                    sum += 1;

                }
            }

        }
        // cout << isS << " " ;
    }

    cout << sum << endl;






}
