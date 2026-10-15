#include "ImprisonedXII.h"
#include <bits/stdc++.h>
using namespace std;

vector<int> sakiko_question(int N, vector<int> a){
    // sort(a.begin(), a.end());
    int count_1 = 0;
    int count_2 = 0;
    vector<int> ans;
    for (int x : a){
        if (x == 1) count_1++;
        else if (x == 2) count_2++;
    }
    while (count_1 > 0 || count_2 > 0){
        if (count_1){
            ans.push_back(1);
            count_1--;

        }
        if (count_2){
            ans.push_back(2);
            count_2--;
        }
    }
    return ans;
}