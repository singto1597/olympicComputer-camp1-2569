#ifndef C1T45_H
#define C1T45_H
#include <iostream>

struct Node
{
    int data;
    Node *next;
};

extern Node *head;

Node *createNode(int data);
void printList();

void push_back(int data);
void push_front(int data);
void deleteNodefront();
void deleteNodeback();

#endif