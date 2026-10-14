#include <iostream>
#include <string>

using namespace std;

struct node{
    string Codename;
    int InternalEnergy;
    node *next;
};

node* createNode(const string &Codename,int InternalEnergy){
    node* newNode = new node;
    newNode->Codename = Codename;
    newNode->InternalEnergy = InternalEnergy;
    newNode -> next = nullptr;
    return newNode;
}

node *head;

void Add(const string &Codename,int InternalEnergy){
//เขียนฟังก์ชันในนี้
}

void Kill(const string &Codename){
//เขียนฟังก์ชันในนี้
}

void MoveToFirst(int requirement){
//เขียนฟังก์ชันในนี้
}

void MoveToLast(const string &Codename){
//เขียนฟังก์ชันในนี้
}

void FindAvailable(int requirement){
//เขียนฟังก์ชันในนี้
}
