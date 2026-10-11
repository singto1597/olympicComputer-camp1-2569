#include <bits/stdc++.h>
using namespace std;

int main (){
    int numberOfList;
    int numberOfTarget;
    cin >> numberOfList >> numberOfTarget;
    int listOfNumber[numberOfList] = {};
    for (int i = 0; i < numberOfList; i++){
        int buffer;
        cin >> buffer;
        listOfNumber[i] = buffer;
    }
    sort(listOfNumber, listOfNumber + numberOfList);

    int index = 0;
    
    while (index < numberOfTarget){
        int target;
        cin >> target;
        cout << upper_bound(listOfNumber, listOfNumber + numberOfList, target) - listOfNumber << endl;
        index++;
    }

}