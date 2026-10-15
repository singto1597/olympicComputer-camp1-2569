#include "everscape.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

namespace {
const int CALLS_CNT_LIMIT = 1000000;

int calls_cnt = 0;
int N,subtask_id;
int A[20005];

void quit(const char* message) {
  printf("%s\n", message);
  fflush(stdout);
  exit(0);
}
} // namespace

int ask(int a, int b, int c) {
  calls_cnt++;
  if (calls_cnt > CALLS_CNT_LIMIT) {
    quit("Too many calls");
  }
  if (a < 1 || a > N || b < 1 || b > N || c < 1 || c > N || a == b || b == c || a == c) {
    quit("Invalid argument");
  }
  int c1 = 0,c2 = 0;
  A[a] ? c1++ : c2++;
  A[b] ? c1++ : c2++;
  A[c] ? c1++ : c2++;
  return c1 >= c2 ? 1 : 0;
}

int main() {
  assert(2 == scanf("%d %d", &N, &subtask_id));
  for (int i = 1; i <= N; i++) {
    assert(1 == scanf("%d", &A[i]));
  }
  fclose(stdin);
  vector<int> R = kanata_flash_drive(N);
  for (int i = 0; i < (int)R.size(); i++) {
    printf("%d%c", R[i], i + 1 == (int)R.size() ? '\n' : ' ');
  }
  printf("%s\n", calls_cnt);
  fclose(stdout);
  return 0;
}