#include <bits/stdc++.h>
using namespace std;

int main (){
    char inputString[3000] = "";
    cin >> inputString;
    int i = 0;

    char *firstIndexPointer = inputString;


    while (*firstIndexPointer != '\0'){
        if (i % 2 == 0){ 
            *firstIndexPointer = toupper(*firstIndexPointer);
        }
        else {
            *firstIndexPointer = tolower(*firstIndexPointer);
        }
        firstIndexPointer++;
        i++;
    }

    cout << inputString << endl;


}