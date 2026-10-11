#include <bits/stdc++.h>

using namespace std;

int m[50][50];
int next_m[50][50];
int main(){
    
    int n, q;
    cin >> n >> q;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){

            cin >> m[i][j];
            next_m[i][j] = m[i][j];
        
        }
       
    }
    for (int _ = 0; _ < q; _++){
        char c;
        cin >> c;
        if (c == 'r'){
            for (int i = 0; i < n; i++){
                for (int j = 0; j < n; j++){
                    next_m[j][n - i - 1] = m[i][j];
                }
            }
        }

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                m[i][j] = next_m[i][j];
                cout << m[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
       
    }
    


}