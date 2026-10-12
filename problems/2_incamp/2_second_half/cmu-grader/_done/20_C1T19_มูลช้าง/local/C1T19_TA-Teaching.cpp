#include <bits/stdc++.h>
using namespace std;


int recursive_function(int number){
    if (number == 1){
        return 0;
    }
    int bestCount = 1e9;
    int count = 0;
    for (int i = 1; i < number; i ++){
        if (number % i == 0){
            count = recursive_function(number - i);
            if (count < bestCount){
                bestCount = count;
            }
            

        }
    }
    return bestCount + 1;

}

int main (){
    int inputNumber;
    cin >> inputNumber;
    int count = recursive_function(inputNumber);
    cout << count;

}