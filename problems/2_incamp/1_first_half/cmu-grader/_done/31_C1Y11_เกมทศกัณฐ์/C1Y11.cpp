#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n;
    cin >> n;
    int team1[n] = {};
    int team2[n] = {};
    for (int i = 0; i < n; i++){
        cin >> team1[i];
    }
    for (int i = 0; i < n; i++){
        cin >> team2[i];
    }
    int pointTeam1 = 0, pointTeam2 = 0;
    for (int i = 0; i < n; i++){
        if (team1[i] > team2[i]){
            pointTeam1 += 2;
        }
        else if (team1[i] < team2[i]){
            pointTeam2 += 2;
        }
        else{
            pointTeam1 += 1;
            pointTeam2 += 1;
        }
    }
    if (pointTeam1 > pointTeam2){
        cout << "1" << endl;
        cout << pointTeam1 << " " << pointTeam2 << endl;
    }
    else if(pointTeam1 < pointTeam2){
        cout << "2" << endl;
        cout << pointTeam2 << " " << pointTeam1 << endl;
    }
    else{
        cout << "D" << endl;
        cout << pointTeam1 << " " << pointTeam2 << endl;
    }
    return 0;
    
}