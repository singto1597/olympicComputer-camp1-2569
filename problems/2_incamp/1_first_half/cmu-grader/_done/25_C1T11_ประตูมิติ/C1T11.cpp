#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int matrix_1_size_m, matrix_1_size_n;
    cin >> matrix_1_size_m >> matrix_1_size_n;

    vector<vector<int>> matrix_1(matrix_1_size_m, vector<int>(matrix_1_size_n, 0));

    int matrix_2_size_m, matrix_2_size_n;
    cin >> matrix_2_size_m >> matrix_2_size_n;

    vector<vector<int>> matrix_2(matrix_2_size_m, vector<int>(matrix_2_size_n, 0));

    if (matrix_1_size_n != matrix_2_size_m){
        cout << "No Solution" << endl;
        return 0;
    }

    vector<vector<int>> matrix_ans(matrix_1_size_m, vector<int>(matrix_2_size_n, 0));

    for (int i = 0; i < matrix_1_size_m; i++){
        for (int j = 0; j < matrix_1_size_n; j++){
            cin >> matrix_1[i][j];
        }
    }

    for (int i = 0; i < matrix_2_size_m; i++){
        for (int j = 0; j < matrix_2_size_n; j++){
            cin >> matrix_2[i][j];
        }
    }

    for (int i = 0; i < matrix_1_size_m; i++){
        for (int j = 0; j < matrix_2_size_n; j++){
            for (int k = 0; k < matrix_1_size_n; k++){
                matrix_ans[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
        }
    }

    for (int i = 0; i < matrix_1_size_m; i++){
        for (int j = 0; j < matrix_2_size_n; j++){
            cout << matrix_ans[i][j] << " ";
        }
        cout << endl;
    }



}
