#include <bits/stdc++.h>

using namespace std;

vector<int> marked_arr;
int count_marked;

bool find(int n){
    // int mid = marked_arr.size() / 2;
    // int left = 0;
    // int right = marked_arr.size() - 1;
    // bool isFound = false;
    // int count = 0;
    // while (left != right){
    //     if (n == marked_arr[mid]){
    //         return 1;
    //         break;
    //     }
    //     else if (n < marked_arr[mid]){
    //         right = mid;
    //     }
    //     else {
    //         left = mid;
    //     }
    //     mid = (right + left) / 2;
    //     // if (left == right || right == left + 1 && !isFound){
    //     //     count++;
    //     //     if (count > 5) break;
    //     // }
    // }

    for (int x : marked_arr){
        if ( n == x) return true;
    }

    return false;
}

void insert(int n){
    if (!find(n)){
        marked_arr.push_back(n);
        count_marked++;
        sort(marked_arr.begin(), marked_arr.end());
    }
}

int main(){

    int t;
    cin >> t;

    while(t--){
        int n, d1, d2;

        cin >> n >> d1 >> d2;
        if (d1 == 1){
            cout << "0" << endl;
            continue;
        }
        count_marked = 0;

        // unordered_set<int> marked;


        // marked.insert(0);
        // marked_arr.push_back(0);
        insert(0);

        int current_1 = d1, current_2 = d2;

        current_1 %= n;
        current_2 %= n;
        // marked.insert(current_1);
        // marked.insert(current_2);
        // marked_arr.push_back(current_1);
        // marked_arr.push_back(current_2);

        insert(current_1);
        insert(current_2);




        while(current_1 != 0 || current_2 != 0){
            current_1 += d1;
            current_2 += d2;
            current_1 %= n;
            current_2 %= n;
            insert(current_1);
            insert(current_2);

            // marked.insert(current_1);
            // marked.insert(current_2);

        }
        marked_arr.resize(0);

        cout << n - count_marked << endl;


    }

    // int n;
    // cin >> n;

    // for (int i = 0; i < n; i++){
    //     int a;
    //     cin >> a;

    //     insert(a);
    // }

    // for (int i = 0; i < n; i++){
    //     cout << marked_arr[i] << " ";
    // }

    // int target;

    // cin >> target;

    // cout << find(target) << endl;

}
