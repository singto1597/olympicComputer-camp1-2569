#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next = NULL;
};


Node* head = nullptr;


void insertByIndex(int idx,int data) {
    Node *cur = head;

    if (idx == 0){
        Node *newNode = new Node{data, head};
        head = newNode;
        return;
    }

    for (int i = 0; i < idx - 1; i++){
        if (cur -> next == NULL) break;
        cur = cur -> next;
    }
    Node *newNode = new Node{data, cur -> next};
    cur -> next = newNode;

}



void printList() { 
    Node * temp = head;
    while(temp != NULL) { 
        printf("%d", temp -> data); 
        if(temp -> next != NULL) { 
            printf(" -> "); 
        } 
        temp = temp -> next; 
    } 
    printf("\n"); 
    return ; 
}


int main() {
    insertByIndex(0, 45);
    insertByIndex(0, 50);
    insertByIndex(0, 60);
    insertByIndex(0, 70);
    insertByIndex(0, 80);
    insertByIndex(4, 40);
    printList();

    return 0;
}