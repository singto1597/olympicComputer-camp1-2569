#include "WitchHunt.h"

Node* mode1(Node* head, int m) {
    if (!head) {
      return nullptr;
    }
    Node *temp = head;
    if (m == 1){
        while(temp->next != head){
            temp = temp->next;
        }
        return temp;
    }

    while (head->next != head){
        // Node *temp_print = head;
        // while(temp_print->next != head){
        //   cout << temp_print->val << " ";
        //   temp_print = temp_print->next;
        // }
        // cout << endl;

        for (int i = 1; i < m - 1; i++){
            temp = temp->next;

        }
        Node *cur = temp->next->next;
        if (temp->next == head){
            head = cur;
        }
        temp->next = cur;
        temp = cur;

    }


    return head;
}









Node* mode2(Node* head, int m, int k, const std::vector<int>& p) {
    return head;
}
