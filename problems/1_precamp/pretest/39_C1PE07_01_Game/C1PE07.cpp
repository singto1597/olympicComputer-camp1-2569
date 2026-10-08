#include <bits/stdc++.h>

using namespace std;

int dx[8] = {-1, -1, -1,  0,  0,  1,  1,  1};
int dy[8] = {-1,  0,  1, -1,  1, -1,  0,  1};

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int T;
    cin >> T;
    
    for (int _ = 0; _ < T; _++){
        int K;
        cin >> K;

        long long total_move = 0;

        for (int i = 0; i < K; i++){
            int n;
            cin >> n;

            long long count0 = 0;
            long long count1 = 0;

            for (int j = 0; j < n; j++){
                long long a;
                cin >> a;

                while(a > 0){
                    if (a % 2 == 0){
                        count0++;
                    }
                    else{
                        count1++;
                    }
                    a /= 2;
                }
            }
            total_move += min(count0, count1);
        }
        if (total_move % 2 != 0){
            cout << "W" << endl;
        }
        else{
            cout << "L" << endl;
        }

    }

}
