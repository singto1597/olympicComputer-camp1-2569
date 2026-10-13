#include "C1T49.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>

namespace {
const int CALLS_CNT_LIMIT = 1000000;

int calls_cnt = 0;
int N;
int A[200005];

void quit(const char* message) {
  printf("%s\n", message);
  fflush(stdout);
  exit(0);
}
} // namespace

int ask(int i, int j) {
  calls_cnt++;
  if (calls_cnt > CALLS_CNT_LIMIT) {
    quit("Too many calls");
  }
  if (i < 0 || i >= N || j < 0 || j >= N) {
    quit("Invalid argument");
  }
  return (A[i] > A[j]) ? i : j;
}

int main() {
  assert(1 == scanf("%d", &N));
  for (int i = 0; i < N; i++) {
    assert(1 == scanf("%d", &A[i]));
  }
  fclose(stdin);
  int mn = -1;
  int mx = -1;
  answer(N, mn, mx);
  printf("%d %d %d\n", mn, mx, calls_cnt);
  fclose(stdout);
  return 0;
}