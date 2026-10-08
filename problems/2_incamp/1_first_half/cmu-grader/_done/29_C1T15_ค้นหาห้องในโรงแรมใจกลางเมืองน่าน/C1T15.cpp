#include <bits/stdc++.h>

using namespace std;

int char_to_int(char c){
    return c - '0';
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    string N;

    cin >> N;

    while (N.size() < 5){
        N = "0" + N;
    }

    string ans = "XXX";

    if      (char_to_int(N[0]) > 5) ans[0] = '1';
    else if (char_to_int(N[1]) > 5) ans[0] = '2';
    else if (char_to_int(N[2]) > 5) ans[0] = '3';
    else if (char_to_int(N[3]) > 5) ans[0] = '4';
    else if (char_to_int(N[4]) > 5) ans[0] = '5';
    else                            ans[0] = '0';

    string re = N;
    reverse(re.begin(), re.end());

    if (N == re){
        if      (char_to_int(N[0]) + char_to_int(N[4]) > 5) ans[1] = '1';
        else if (char_to_int(N[1]) * char_to_int(N[3]) > 5) ans[1] = '2';
        else                                                ans[1] = '0';
    }
    else{
        if (char_to_int(N[4]) != 0){
            if      (char_to_int(N[0]) / char_to_int(N[4]) > 5) ans[1] = '1';
            else if (char_to_int(N[1]) - char_to_int(N[4]) > 5) ans[1] = '2';
            else                                                ans[1] = '0';

        }
        else{
            if (char_to_int(N[1]) - char_to_int(N[4]) > 5)  ans[1] = '2';
            else                                            ans[1] = '0';
        }
    }

    if(
        char_to_int(N[0]) +
        char_to_int(N[1]) +
        char_to_int(N[2]) +
        char_to_int(N[3]) +
        char_to_int(N[4]) > 25
    ){
        ans[2] = '1';
    }
    else if(
        char_to_int(N[0]) *
        char_to_int(N[1]) *
        char_to_int(N[2]) *
        char_to_int(N[3]) *
        char_to_int(N[4]) > 55

    ){
        ans[2] = '2';
    }
    else{
        ans[2] = '0';
    }


    cout << ans << endl;


}
