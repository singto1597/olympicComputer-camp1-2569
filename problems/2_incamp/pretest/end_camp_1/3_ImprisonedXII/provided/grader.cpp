#include "ImprisonedXII.h"
#include <cassert>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
  int t;
  assert(1 == scanf("%d", &t));
  for (int i = 0; i < t; i++) {
    int N;
    assert(1 == scanf("%d", &N));
    vector<int> a(N);
    for(int j = 0; j < N; j++) {
      assert(1 == scanf("%d", &a[j]));
    }
    vector<int> b = sakiko_question(N, a);
    for(int j = 0; j < (int)b.size(); j++) {
      printf("%d%c", b[j], j + 1 == (int)b.size() ? '\n' : ' ');
    }
  }
  fclose(stdin);
  fclose(stdout);
  return 0;
}