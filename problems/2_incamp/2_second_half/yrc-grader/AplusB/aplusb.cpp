#include "aplusb.h"

int arr1[200005];
int arr2[200005];

void initialize(int N, int A[], int B[]) {
  for (int i = 0; i < N; i++){
    arr1[i] = A[i];
    arr2[i] = B[i];
  }
  return;
}

int answer_question(int i, int j) {
  return arr1[i] + arr2[j];
}
