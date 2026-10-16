#include <cstdio>
#include <vector>

struct Node {
  int val;
  Node* next;
};

Node* createNode(int v);

Node* mode1(Node* head, int m);

Node* mode2(Node* head, int m, int k, const std::vector<int>& p);
