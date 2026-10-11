#include <bits/stdc++.h>
using namespace std;

int main (){
    int hourIn;
    int minutesIn;
    int hourOut;
    int minutesOut;

    cin >> hourIn >> minutesIn;
    cin >> hourOut >> minutesOut;
    int allMinutesInFromZero = hourIn * 60 + minutesIn;
    int allMinutesOutFromZero = hourOut * 60 + minutesOut;

    int price = 0;
    
    int allTime = allMinutesOutFromZero - allMinutesInFromZero;

    // if (allTime > 6 * 60){
    //     price = 200;
    // }
    // else if (allTime > 3 * 60 && allTime <= 6 * 60){
    //     if ((allTime) % 60 != 0){
    //         price += ((allTime) / 60 + 1) * 20;

    //     }
    //     else{
    //         price += ((allTime) / 60) * 20;
    //     }
    // }
    // else if (allTime > 15 && allTime <= 3 * 60){
    //     if ((allTime) % 60 != 0){
    //         price += ((allTime) / 60 + 1) * 10;

    //     }
    //     else{
    //         price += ((allTime) / 60) * 10;
    //     }
    // }
    // else if (allTime <= 15){
    //     price = 0;
    // }

    if (allTime <= 15){
        price = 0;
    }
    else if (allTime <= 180){
        if (allTime % 60 == 0){
            price = (allTime / 60) * 10;
        }
        else{
            price = (allTime / 60 + 1) * 10;
        }
    }
    else if (allTime <= 360){
        if (allTime % 60 == 0){
            price = ((allTime - 180) / 60) * 20 + 30;
        }
        else{
            price = ((allTime - 180) / 60 + 1) * 20 + 30;
        }
    }
    else{
        price = 200;
    }



    cout << price << endl;


}