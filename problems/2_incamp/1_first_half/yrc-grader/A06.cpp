#include <bits/stdc++.h>

using namespace std;

int main(){
    int card_want, card_have;
    cin >> card_want >> card_have;

    int cards_value[card_have + 5] = {};

    for (int i = 0; i < card_have; i++){
        cin >> cards_value[i];
    }

    for (int i = 0; i < card_have ; i++){
        int min = 2000;
        int j_want_swap;
        for (int j = i + 1; j < card_have; j++){
            if (cards_value[j] < min ) {
                min = cards_value[j];
                j_want_swap = j;
            }
        }
        if (cards_value[i] > cards_value[j_want_swap]){
            swap(cards_value[i], cards_value[j_want_swap]);

        }
        // cout << cards_value[i] << " ";
    }

    // cout << endl;

    int min_diff = 200000;

    for (int i = 0; i < card_have - card_want + 1; i++){
        // cout << cards_value[i + card_want] - cards_value[i] << endl;
        if ((cards_value[i + card_want - 1] - cards_value[i]) < min_diff){
            min_diff = (cards_value[i + card_want - 1] - cards_value[i]);
        }
    }

    cout << min_diff << endl;





}
