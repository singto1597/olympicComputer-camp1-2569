#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    long long n;
    cin >> n;

    vector<pair<long long, long long>> pigad;

    for (int i = 0; i < n; i ++){
        long long x, y;
        cin >> x >> y;
        pigad.push_back({x,y});

    }

    long long area = abs((pigad[0].first * pigad[1].second + pigad[1].first * pigad[2].second + pigad[2].first * pigad[0].second) -
                (pigad[0].second * pigad[1].first + pigad[1].second * pigad[2].first + pigad[2].second * pigad[0].first)
                ) / 2;

    cout << area;



}
