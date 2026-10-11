#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    long long h, start_point_L, bomb_amount_N;

    cin >> h >> start_point_L >> bomb_amount_N;

    long long max_bomb = 0;
    long long count = 1;

    long long bomb_position_prev = 0;
    long long max_bomb_position = -1;

    for (long long i = 0; i < bomb_amount_N; i++){
        long long bomb_position;
        cin >> bomb_position;


        // cout << "1: " << count << endl;

        if (bomb_position < start_point_L) {
            count = 0;
            continue;
        }

        if (bomb_position > start_point_L + h){
            count = 0;
            continue;
        }

        if (bomb_position == bomb_position_prev) count++;
        else count = 1;


        // cout << "2: " << count << endl;

        if (count > max_bomb){
            max_bomb = count;
            max_bomb_position = bomb_position;
        }

        bomb_position_prev = bomb_position;
    }

    cout << max_bomb << " " << max_bomb_position << endl;


}
