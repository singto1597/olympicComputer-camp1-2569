#include <iostream>
using namespace std;

int main() {
    int input = 0;
    int evenNumber = 0;
    int oddNumber = 0;
    int maxEvenNumber = 0;
    int maxOddNumber = 0;
    for (int i = 0; i < 10; i++){
        cin >> input;
        if (input % 2 == 0){
            evenNumber++;
            if (input > maxEvenNumber){
                maxEvenNumber = input;
            }
        }
        else {
            oddNumber++;
            if (input > maxOddNumber){
                maxOddNumber = input;
            }
        }

    }
    cout << oddNumber << endl;
    cout << evenNumber << endl;
    cout << maxOddNumber << endl;
    cout << maxEvenNumber << endl;
    return 0;
}