#include <iostream>
#include <string>

using namespace std;

struct Cage {
    int no;
    int capacity;
    string name;
    Cage *next;
};

Cage *first = nullptr;

Cage *createCage(int no, int capacity,const string &name) {
    Cage *newCage = new Cage;
    newCage->no = no;
    newCage->capacity = capacity;
    newCage->name = name;
    newCage->next = nullptr;
    return newCage;
}

//ไม่ต้องเขียนฟังก์ชัน showTour

void showCagesWithCapacityGreaterThanK(int k) {
    Cage *startCage = first;
    while (startCage) {
        if (startCage->capacity > k){
            cout << startCage->no << " ";

        }
        startCage = startCage->next;
    }
    cout << endl;
}

void movetofirst(int no) {
    Cage *temp = first;
    if (!first){
        return;
    }
    if (temp->no == no){
        return;
    }
    while(temp->next && temp->next->no != no){
        temp = temp->next;
    }
    if (temp->next->no == no){
        Cage *target = temp->next;
        temp->next = target->next;
        target->next = first;
        first = target;
    }


}


void insertLast(int no, int capacity, const string &name) {
    Cage *newNode = new Cage{no, capacity, name, nullptr};
    if (!first){
        first = newNode;
        return;
    }
    Cage *temp = first;
    while (temp->next){
        temp = temp->next;
    }
    temp->next = newNode;
    return;


}

void shuffle(int no) {
    //Your Code Here
    Cage *temp = first;
    if (!first){
        return;
    }
    if (!first->next){
        return;
    }
    if (first->no == no){
        return;
    }

    while(temp->next && temp->next->no != no){
        temp = temp->next;
    }

    if (!temp->next){
        return;
    }


    if (temp->next->no == no){
        Cage *target = temp->next;
        Cage *last = target;
        while (last->next){
            last = last->next;
        }
        temp->next = nullptr;
        last->next = first;
        first = target;
    }
}


void deleteCage(const string &s) {
    if (!first){
        return;
    }
    Cage *temp = first;
    if (first->name == s){
        first = first->next;
        delete temp;
    }

    while(temp->next && temp->next->name != s){
        temp = temp->next;
    }

    if (!temp->next){
        return;
    }

    if (temp->next->name == s){
        Cage *target = temp->next;
        temp->next = target->next;
        delete target;
    }


}
