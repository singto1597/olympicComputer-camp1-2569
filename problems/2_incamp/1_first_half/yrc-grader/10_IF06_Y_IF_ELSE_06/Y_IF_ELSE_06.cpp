#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int d1, m1, y1;
    cin >> d1 >> m1 >> y1;


    int day_in_mount[13] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


    y1 -= 543;

    if (y1 % 4 == 0) day_in_mount[2] += 1;
    if (y1 % 100 == 0) day_in_mount[2] -= 1;
    if (y1 % 400 == 0) day_in_mount[2] += 1;

    if (d1 > day_in_mount[m1] || m1 > 12 || m1 < 1 || d1 < 1) {
        cout << "INVALID" << endl;
        return 0;
    }


    d1 += 1;

    if (d1 > day_in_mount[m1]){
        d1 = 1;
        m1++;
    }
    if (m1 > 12){
        m1 = 1;
        y1++;
    }

    cout << d1 << " " << m1 << " " << y1 + 543 << endl;



}
