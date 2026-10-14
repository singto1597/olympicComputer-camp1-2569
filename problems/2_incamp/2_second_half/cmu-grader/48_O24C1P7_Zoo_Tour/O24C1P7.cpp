
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


// 1. ว่าง
// 2. หัว
// 3. ท้าย
// 4. ระหว่าง Node

Cage *createCage(int no, int capacity, const string &name) {
    Cage *newCage = new Cage;
    newCage->no = no;
    newCage->capacity = capacity;
    newCage->name = name;
    newCage->next = nullptr;
    return newCage;
}


void showTour() {
    Cage *startCage = first;
    while (startCage) {
        cout << startCage->name << " ";
        startCage = startCage->next;
    }
    cout << endl;
}

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

    while(temp->next && temp->next->no != no){
        temp = temp->next;
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

}

int main() {
    insertLast(10, 5, "moodang");
    insertLast(20, 6, "moodeng");
    insertLast(30, 7, "moohan");
    insertLast(40, 8, "mooyang");
    insertLast(50, 9, "mookrob");
    showTour();
    shuffle(50);
    showTour();
    showCarriagesWithCapacityGreaterThanK(6);
    deleteCage("moohan");
    showTour();
    return 0;
}
