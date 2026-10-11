#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
};


Node* head = nullptr;


Node* createNode(int data) {
    //Your Code
    Node *newNode = new Node();
    newNode -> data = data;
    newNode -> next = NULL;
    return newNode;
}


void insertFirst(int data) {
    //Your Code
    Node *firstNode = createNode(data);
    firstNode -> next = head;
    head = firstNode;

}