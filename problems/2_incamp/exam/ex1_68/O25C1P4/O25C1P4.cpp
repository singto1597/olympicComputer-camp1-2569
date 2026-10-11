#include <bits/stdc++.h>

using namespace std;

char commands_move_pos_1[200];
int commands_move_amount_1[200];
char commands_move_pos_2[200];
int commands_move_amount_2[200];

long long tables[3009][3009] = {};
int main(){
    char command_output;
    cin >> command_output;

    int row_pos_1, col_pos_1;
    int commands_amount_1;

    cin >> row_pos_1 >> col_pos_1;
    cin >> commands_amount_1;

    int move_1_amount = 0;
    long long treasure_1 = 0;

    for (int i = 0; i < commands_amount_1; i++){
        cin >> commands_move_pos_1[i];
        cin >> commands_move_amount_1[i];
        move_1_amount += commands_move_amount_1[i];
    }


    int row_pos_2, col_pos_2;
    int commands_amount_2;

    cin >> row_pos_2 >> col_pos_2;
    cin >> commands_amount_2;

    int move_2_amount = 0;
    long long treasure_2 = 0;

    for (int i = 0; i < commands_amount_2; i++){
        cin >> commands_move_pos_2[i];
        cin >> commands_move_amount_2[i];
        move_2_amount += commands_move_amount_2[i];
    }



    int treasures_amount;
    cin >> treasures_amount;

    for (int i = 0; i < treasures_amount; i++){
        int rt, ct;
        cin >> rt >> ct;

        long long tresure_value;
        cin >> tresure_value;

        tables[rt][ct] = tresure_value;
    }



    int total_move = max(move_1_amount, move_2_amount);

    int curr_index_1 = 0;
    int curr_index_2 = 0;

    while (total_move > 0){

        if (row_pos_1 != row_pos_2 || col_pos_1 != col_pos_2) {
            if (tables[row_pos_1][col_pos_1] > 0){
                treasure_1 += tables[row_pos_1][col_pos_1];
                tables[row_pos_1][col_pos_1] = 0;
            }
            if (tables[row_pos_2][col_pos_2] > 0){
                treasure_2 += tables[row_pos_2][col_pos_2];
                tables[row_pos_2][col_pos_2] = 0;
            }
        }
        else{
            if (tables[row_pos_1][col_pos_1] > 0){
                treasure_1 += tables[row_pos_1][col_pos_1] / 2;
                treasure_2 += tables[row_pos_1][col_pos_1] / 2;
                tables[row_pos_1][col_pos_1] = 0;
            }
        }

        //    N
        //  W   E
        //    S

        if (move_1_amount > 0){
            switch (commands_move_pos_1[curr_index_1]){
                case 'N':
                    row_pos_1--;
                    break;
                case 'S':
                    row_pos_1++;
                    break;
                case 'W':
                    col_pos_1--;
                    break;
                case 'E':
                    col_pos_1++;
                    break;
            }
            commands_move_amount_1[curr_index_1]--;
            if (commands_move_amount_1[curr_index_1] <= 0) curr_index_1++;
        }


        if (move_2_amount > 0){
            switch (commands_move_pos_2[curr_index_2]){
                case 'N':
                    row_pos_2--;
                    break;
                case 'S':
                    row_pos_2++;
                    break;
                case 'W':
                    col_pos_2--;
                    break;
                case 'E':
                    col_pos_2++;
                    break;
            }
            commands_move_amount_2[curr_index_2]--;
            if (commands_move_amount_2[curr_index_2] <= 0) curr_index_2++;
        }

        total_move--;
        move_1_amount--;
        move_2_amount--;
    }

    if (row_pos_1 != row_pos_2 || col_pos_1 != col_pos_2) {
            if (tables[row_pos_1][col_pos_1] > 0){
                treasure_1 += tables[row_pos_1][col_pos_1];
                tables[row_pos_1][col_pos_1] = 0;
            }
            if (tables[row_pos_2][col_pos_2] > 0){
                treasure_2 += tables[row_pos_2][col_pos_2];
                tables[row_pos_2][col_pos_2] = 0;
            }
        }
        else{
            if (tables[row_pos_1][col_pos_1] > 0){
                treasure_1 += tables[row_pos_1][col_pos_1] / 2;
                treasure_2 += tables[row_pos_1][col_pos_1] / 2;
                tables[row_pos_1][col_pos_1] = 0;
            }
        }



    if (command_output == 'V'){

        cout << row_pos_1 << " " << col_pos_1 << " " << treasure_1 << endl;
        cout << row_pos_2 << " " << col_pos_2 << " " << treasure_2 << endl;
    }
    else{
        cout << row_pos_1 << " " << col_pos_1 << endl;
        cout << row_pos_2 << " " << col_pos_2 << endl;
    }


}
