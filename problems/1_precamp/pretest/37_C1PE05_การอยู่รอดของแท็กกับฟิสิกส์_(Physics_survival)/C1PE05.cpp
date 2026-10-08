#include <bits/stdc++.h>

using namespace std;

long long dist2(pair<long long, long long> p1, pair<long long, long long> p2) {
    long long dx = p1.first - p2.first;
    long long dy = p1.second - p2.second;
    return dx * dx + dy * dy;
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int n;
    cin >> n;

    vector<pair<long long, long long>> pigad;

    for (int i = 0; i < n; i++){
        long long x,y;
        cin >> x >> y;
        pigad.push_back({x,y});
    }

    long long count = 0;

    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            for (int k = j + 1; k < n; k++){
                for(int l = k + 1; l < n; l++){
                    long long d[6];
                    d[0] = dist2(pigad[i], pigad[j]);
                    d[1] = dist2(pigad[i], pigad[k]);
                    d[2] = dist2(pigad[i], pigad[l]);
                    d[3] = dist2(pigad[j], pigad[k]);
                    d[4] = dist2(pigad[j], pigad[l]);
                    d[5] = dist2(pigad[k], pigad[l]);

                    sort(d, d + 6);
                    if (d[0] > 0 && d[0] == d[1] && d[1] == d[2] && d[2] == d[3] && d[4] == d[5]) {
                        count++;
                    }
                }
            }
        }
    }

    cout << count << endl;

}
