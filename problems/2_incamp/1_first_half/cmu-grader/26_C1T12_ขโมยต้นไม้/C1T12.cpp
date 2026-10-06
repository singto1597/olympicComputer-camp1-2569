#include <bits/stdc++.h>
using namespace std;

int number_of_tree, max_weight_of_bag;

vector<int> list_value_kwam_souy_ngam;
vector<int> weight_of_tree;

int find_max_tree(int n, int weight_left){
    // cout << n << " " << weight_left << endl;
    if (n == 0 || weight_left <= 0){
        return 0;
    }

    if (weight_of_tree[n - 1] > weight_left){
        return find_max_tree(n - 1, weight_left);
    }

    return max (list_value_kwam_souy_ngam[n - 1] + find_max_tree(n - 1, weight_left - weight_of_tree[n - 1]),
                find_max_tree(n - 1, weight_left));
}

int main() {
    cin >> number_of_tree >> max_weight_of_bag;

    list_value_kwam_souy_ngam.resize(number_of_tree, 0);
    weight_of_tree.resize(number_of_tree, 0);

    for(int i = 0; i < number_of_tree; i++){
        int value_kwam_souy_ngam_temp, weight_of_tree_temp;
        cin >> value_kwam_souy_ngam_temp >> weight_of_tree_temp;
        list_value_kwam_souy_ngam[i] = value_kwam_souy_ngam_temp;
        weight_of_tree[i] = weight_of_tree_temp;
    }

    cout << find_max_tree(number_of_tree, max_weight_of_bag) << endl;

    return 0;

}
