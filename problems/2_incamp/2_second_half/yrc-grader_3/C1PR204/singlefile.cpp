#include <iostream>
#include <string>
using namespace std;

struct node{
    string Slayer;
    node *next;
};

node* createNode(const string &Slayer){
    node *newNode = new node();
    newNode->next = nullptr;
    newNode->Slayer = Slayer;
    return newNode;
}

node *head = nullptr;

void deleteRange(int l,int r){
    // node *cur = head;
    // for (int i = 0; i < l; i++){
    //     cur = cur->next;
    // }

}


void shuffle(const string &Slayer,const string &Slayer2){
//เขียนฟังก์ชันในนี้
}

void push(const string &Slayer){
    node *temp = head;
    while (temp->next && temp->next->Slayer != Slayer){
        temp = temp->next;
    }
    if (temp->next->Slayer == Slayer){
        node *temp_before = temp;
        node *target = temp->next;
        node *temp_after = temp->next->next;
        temp_before->next = temp_after;
        target->next = temp_after->next;
        temp_after->next = target;
    }
}



void insert(const string &Slayer){
    node *newNode = createNode(Slayer);
    if (!head){
        head = newNode;
        return;
    }
    node *temp = head;
    while (temp->next){
        temp = temp->next;
    }
    temp->next = newNode;
    return;
}

void peek(){
    node *temp = head;
    while(temp != nullptr){
        cout << temp->Slayer << ' ';
        temp = temp->next;
    }
    cout << endl;
}

int main(){
    insert("Tanjiro");
    insert("Zenitsu");
    insert("Inosuke");
    insert("Kanao");
    insert("Giyu");
    peek();
    push("Zenitsu");
    peek();
    push("Giyu");
    peek();
    shuffle("Tanjiro","Kanao");
    peek();
    deleteRange(0,2);
    peek();
}
