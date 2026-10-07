#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    long long input_head;
    cin >> input_head;

    string binary_head = "";

    while (input_head){
        binary_head = (char)(input_head % 2) + binary_head;
        input_head /= 2;
    }

    binary_head

}
