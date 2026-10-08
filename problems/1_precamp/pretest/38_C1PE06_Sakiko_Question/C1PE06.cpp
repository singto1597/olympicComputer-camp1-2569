#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    long long n;
    cin >> n;

    for (int _ = 0; _ < n; _++){
        string s;

        cin >> s;

        bool isFound = false;

        for (int i = 0; i < s.size(); i++){
            int left = i;
            int right = i;

            while (left >= 0 && right < s.size() && s[left] == s[right]){
                if (right - left + 1 >= 2){
                    isFound = true;
                }
                left--;
                right++;
            }

            left = i;
            right = i+1;

            while (left >= 0 && right < s.size() && s[left] == s[right]){
                isFound = true;

                left--;
                right++;
            }
            if (isFound) break;
        }

        if (isFound){
            cout << "YES" << endl;
        }
        else{
            cout << "NO" << endl;
        }


    }

}
