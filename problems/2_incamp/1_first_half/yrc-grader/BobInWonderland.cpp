#include <bits/stdc++.h>

using namespace std;

constexpr int MOD = 1e5 + 7;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    long double circle_x, circle_y;
    long double ratsamee, nukrean;
    cin >> circle_x >> circle_y;
    cin >> ratsamee >> nukrean;



    long double time = 0.00;

    long double count = 0;

    for (int i = 0; i < nukrean; i++){
        long double x , y;
        cin >> x >> y;
        long double d = sqrt((x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y));
        if (d > ratsamee){
            count++;
            time += d - (long double)ratsamee;
        }
    }

    cout << count << " " << fixed << setprecision(9) << time << endl;



}
