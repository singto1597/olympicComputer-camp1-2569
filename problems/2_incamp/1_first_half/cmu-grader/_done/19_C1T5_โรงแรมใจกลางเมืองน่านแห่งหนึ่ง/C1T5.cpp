#include <iostream>
using namespace std;
int main() {
    int day, years, cost, time;
    cin >> day >> years >> time >> cost;
    bool isLeap = false;
    if ((years % 4 == 0 && years % 100 != 0) || years % 400 == 0){
        isLeap = true;
    }
    else {
        isLeap = false;
    }
    int percents = 0;
    
    int totalMinutes = (time / 100) * 60 + (time % 100);
    if (isLeap) {
        if (day > 7) {
        percents += 10;
        }
        else {
            percents += 5;
        }
        if (totalMinutes >= 570 && totalMinutes <= 690) {        // 09:30 - 11:30
            percents += 30;
        } else if (totalMinutes >= 691 && totalMinutes <= 810) { // 11:31 - 13:30
            percents += 20;
        } else if (totalMinutes >= 811 && totalMinutes <= 1110){ // 13:31 - 18:30
            percents += 15;
        } else {
            percents += 10;
        }
    }
    else{
        if (day > 7) {
            percents += 5;
        }
        if (totalMinutes >= 570 && totalMinutes <= 690) {        // 09:30 - 11:30
            percents += 15;
        } else if (totalMinutes >= 691 && totalMinutes <= 810) { // 11:31 - 13:30
            percents += 10;
        } else if (totalMinutes >= 811 && totalMinutes <= 1110){ // 13:31 - 18:30
            percents += 5;
        }
    }
    
    printf ("%.2lf\n", (day * cost) - (day * cost) * ((double)percents / 100.00));

}