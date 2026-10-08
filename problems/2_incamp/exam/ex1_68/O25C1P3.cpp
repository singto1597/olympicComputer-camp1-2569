#include <bits/stdc++.h>

using namespace std;

int main(){

    // ข้อนี้ใช้ Ai ช่วยคิด ไม่ได้คิดเอง 100%

    int n;
    cin >> n;

    int total_level_up = 0;
    int total_super_level_up = 0;

    for (int _ = 0; _ < n; _++){
        int level, power;
        int a,b;

        cin >> level >> power;
        cin >> a >> b;
        //   A : B
        // ซาก : บริสุทธิ์

        long long remains = power * level;
        long long current_power = remains;
        int train_count = 0;
        int level_up_count = 0;

        if (current_power >= 10000) {
            level++;
            level_up_count++;
            current_power = 0;
        }

        while (remains >= a){
            long long new_stone = (remains / a) * b;
            remains = remains % a;

            long long temp_power = new_stone * level;
            current_power += temp_power;
            train_count++;
            remains += new_stone;

            if (current_power >= 10000){
                level += 1;
                level_up_count++;
                current_power = 0;
            }

        }
        cout << train_count << " " << level << " " << current_power;
        if (level_up_count > 0) {
            cout << " " << level_up_count;
        }
        cout << "\n";

        if (level_up_count > 0) {
            total_level_up++;
            if (level_up_count > 1) {
                total_super_level_up++;
            }
        }
    }

    cout << total_level_up;
    if (total_super_level_up > 0) {
        cout << " " << total_super_level_up;
    }
    cout << "\n";




}
