#include "C1T46.h"
using namespace std;

static const int MAX = 100;
static int arr[MAX];
static int topIdx = -1;

bool isEmpty() {
    if (topIdx == -1) return true;
    else return false;
}

bool isFull() {
    if (topIdx == MAX - 1) return true;
    else return false;
}

void push(int value) {
    if (!isFull()){
        topIdx++;
        arr[topIdx] = value;
        return;
    }
    cout << "Stack Overflow\n";
}

void pop() {
    if (isEmpty()){
        cout << "Stack Underflow\n";
        return;
    }
    topIdx--;
}

int peek() {
    if (isEmpty()) {
        cout << "Stack is empty\n";
        return -1;
    }
    return arr[topIdx];
}

int size() {
    return topIdx + 1;
}

void display() {
    if (isEmpty()) {
        cout << "Stack is empty\n";
        return; 
    }
    for (int i = topIdx; i >= 0; i--){
        cout << arr[i] << " ";
    }
    cout << "\n";
}
