#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    long long n;
    cin >> n;

    int max = 25;

    int count = 0;

    bool is_palin = false;



    while(count <= 25){
        string n_str_rev = to_string(n);
        reverse(n_str_rev.begin(), n_str_rev.end());
        string n_str = to_string(n);

        if (n_str == n_str_rev){
            is_palin = true;
            break;
        }
        count++;
        n = stoll(n_str_rev) + n;
    }
    if (count > max){
        cout << "-1" << endl;
        return 0;
    }
    cout << count << " " << n;
}
