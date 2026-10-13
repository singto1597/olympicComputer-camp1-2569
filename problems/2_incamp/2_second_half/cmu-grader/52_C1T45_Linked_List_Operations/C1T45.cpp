#include "C1T45.h"
using namespace std;

void push_back(int data){
    Node *newNode = createNode(data);

    if (!head){
        head = newNode;
        return;
    }

    Node *temp = head;

    while (temp->next != nullptr){
        temp = temp->next;
    }

    temp->next = newNode;
}

void push_front(int data){
    Node *newNode = createNode(data);

    Node *temp = head;

    newNode->next = temp;

    head = newNode;
}

void deleteNodefront(){
    if (!head) return;
    Node *temp = head;
    head = head->next;

    delete temp;
}

void deleteNodeback(){
    if (!head) return;

    if (head->next == nullptr) {
        delete head;
        head = nullptr;
        return;
    }

    Node *temp = head;

    while (temp->next->next != nullptr){
        temp = temp->next;
    }

    Node *target = temp->next;

    temp->next = nullptr;

    delete target;

}
