#include "WitchHunt.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

Node* createNode(int v) {
  Node* newNode = new Node();
  newNode->val = v;
  newNode->next = nullptr;
  return newNode;
}

namespace {

Node* build(int n) {
  Node* head = createNode(1);
  Node* cur = head;
  for (int i = 2; i <= n; i++) {
    cur->next = createNode(i);
    cur = cur->next;
  }
  cur->next = head;
  return head;
}

void quit(const char* message) {
  printf("%s\n", message);
  exit(0);
}
} // namespace





Node* mode1(Node* head, int m) {
    if (!head) {
      return nullptr;
    }
    Node *temp = head;
    if (m == 1){
        while(temp->next != head){
            temp = temp->next;
        }
        return temp;
    }

    while (head->next != head){
        // Node *temp_print = head;
        // while(temp_print->next != head){
        //   cout << temp_print->val << " ";
        //   temp_print = temp_print->next;
        // }
        // cout << endl;

        for (int i = 1; i < m - 1; i++){
            temp = temp->next;

        }
        Node *cur = temp->next->next;
        if (temp->next == head){
            head = cur;
        }
        temp->next = cur;
        temp = cur;

    }


    return head;
}








int main() {
  int mode, N, M;
  assert(1 == scanf("%d", &mode));
  assert(2 == scanf("%d %d", &N, &M));

  Node* head = build(N);
  Node* result = nullptr;

  if (mode == 1) {
    result = mode1(head, M);
  } 
  fclose(stdin);

  if (result == nullptr) {
    quit("Invalid return value");
  }
  printf("%d\n", result->val);

  fclose(stdout);
  return 0;
}
