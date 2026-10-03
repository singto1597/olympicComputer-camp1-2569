#include <iostream>
#include <string>
using namespace std;


int main() {
    int numberOfWatch;
    cin >> numberOfWatch;
    int hourBase, minuteBase;
    char colon;
    cin >> hourBase >> colon >> minuteBase;
    int minutesBaseFromZero ;
    if ((hourBase * 60 + minuteBase) - 12 * 60 < 0){
        minutesBaseFromZero = hourBase * 60 + minuteBase;
    }
    else {
        minutesBaseFromZero = (hourBase * 60 + minuteBase) - 12 * 60;
    }
    // cout << minutesBaseFromZero << endl;
    // int halfOfBase = (minutesBaseFromZero + 30 * 12) %  (12 * 60);
    // cout << halfOfBase / 60 << endl;

    int sumeryOfMinutes = 0;


    for (int i = 0; i < numberOfWatch; i++){
        int hourWatch, minuteWatch;
        cin >> hourWatch >> colon >> minuteWatch;
        int minuteWatchFromBase = hourWatch * 60 + minuteWatch;
        int differentsOfWatchFromBase = abs(minutesBaseFromZero - minuteWatchFromBase);

        int minDifferentsOfWatch = min(differentsOfWatchFromBase, 720 - differentsOfWatchFromBase);
        // cout << minDifferentsOfWatch << endl;
        sumeryOfMinutes += minDifferentsOfWatch;
    }

    
    cout << sumeryOfMinutes << endl;

    return 0;
    
}