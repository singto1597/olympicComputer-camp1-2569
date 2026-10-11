#include <bits/stdc++.h>
using namespace std;


int recursive_function(int numberOfElephantPoop){
    if (numberOfElephantPoop == 1){
        return 0;
    }
    int bestPointOfCount = 1e9;
    int count = 0;
    for (int i = 1; i < numberOfElephantPoop; i++){
        if (numberOfElephantPoop % i == 0){
            count = recursive_function(numberOfElephantPoop - i);
            if (count < bestPointOfCount){
                bestPointOfCount = count;
            }
        }
    }
    return bestPointOfCount + 1;

}

int main (){
    int n;
    cin >> n;
    int count = recursive_function(n);
    cout << count << endl;
}