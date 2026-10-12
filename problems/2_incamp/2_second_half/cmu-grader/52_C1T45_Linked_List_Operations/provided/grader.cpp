#include "C1T45.h"
using namespace std;

Node *head = nullptr;

Node *createNode(int data)
{
    Node *newNode = new Node();

    newNode->data = data;
    newNode->next = nullptr;

    return newNode;
}

void printList()
{
    Node *temp = head;

    while (temp != nullptr)
    {
        cout << temp->data << ' ';
        temp = temp->next;
    }

    cout << '\n';
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--)
    {
        int option;
        cin >> option;

        if (option == 1)
        {
            int data;
            cin >> data;
            push_back(data);
        }
        else if (option == 2)
        {
            int data;
            cin >> data;
            push_front(data);
        }
        else if (option == 3)
        {
            deleteNodefront();
        }
        else if (option == 4)
        {
            deleteNodeback();
        }
        else if (option == 5)
        {
            printList();
        }
    }

    return 0;
}