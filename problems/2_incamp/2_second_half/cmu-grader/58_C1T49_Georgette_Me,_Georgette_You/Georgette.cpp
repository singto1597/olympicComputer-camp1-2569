#include "Georgette.h"


#include <bits/stdc++.h>

using namespace std;

void answer(int N, int &mn, int &mx) {
  vector<int> winners;
  vector<int> losers;

  for (int i = 0; i < N - 1; i += 2){
    int larger = ask(i, i + 1);
    if (larger == i){
      winners.push_back(i);
      losers.push_back(i + 1);
    }
    else{
      winners.push_back(i + 1);
      losers.push_back(i);
    }



  }

  if (N % 2 == 1){
    winners.push_back(N - 1);
    losers.push_back(N - 1);
  }

  mx = winners[0];

  for (int i = 1; i < winners.size(); i++){
    mx = ask(mx, winners[i]);
  }

  mn = losers[0];

  for (int i = 1; i < losers.size(); i++){
    int larger = ask(mn, losers[i]);

    if (larger == mn){
      mn = losers[i];
    }
  }

  return;
}
