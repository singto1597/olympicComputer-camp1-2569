#include <iostream>
using namespace std;
int main() {
    // int n , m;
    // cin >> n >> m;
    // int int_ = 0;
    // char char_ = 'A';
    // bool left = true;

    // for (int i = 0, line = 0; line < n; i++){
    //     if (i % m == 0 && i != 0){
    //         left = !left;
    //         continue;
    //     } 
    //     line ++;
    //     for (int j = 0; j < m; j++){
    //         if ((i - j) % m == 0 && (i + j) % m == m-1){
    //             cout << char_;
    //             char_++;
    //             int_++;
    //             if(char_ > 'Z') char_ = 'A';
    //         }
            
    //         else if ((i - j) % m == 0) {
    //             if (left){
    //                 cout << char_;
    //                 char_++;
    //                 if(char_ > 'Z') char_ = 'A';
    //             }
    //             else {
    //                 cout << int_;
    //                 int_++;
    //                 if (int_ > 9) int_ = 0;
    //             }
    //         }
    //         else if ((i + j) % m == m-1) {
    //             if (!left){
    //                 cout << char_;
    //                 char_++;
    //                 if(char_ > 'Z') char_ = 'A';
    //             }
    //             else {
    //                 cout << int_;
    //                 int_++;
    //                 if (int_ > 9) int_ = 0;
    //             }
    //         }
    //         else cout << " ";
    //     }
    //     cout << endl;
    // }

    int row, column;
    cin >> row >> column;
    int number = 0;
    char character = 'A';
    bool reflex = true;
    if (column == 1){
        for (int i = 0; i < row; i++){
            cout << character << endl;
            character++;
            if(character > 'Z') character = 'A';
        }
        return 0;
    }
    for (int i = 0, line = 0; line < row; i++){
        if (i % column == 0 && i != 0){
            reflex = !reflex;
            continue;
        }
        line++;
        
        for (int j = 0; j < column; j++){

            if ((i - j) % column == 0 && (i + j) % column == column-1){
                cout << character;
                character++;
                number++;
                if(character > 'Z') character = 'A';
                if (number > 9) number = 0;
            }

            else if ((i - j) % column == 0){
                if (reflex){
                    cout << character;
                    character++;
                    if (character > 'Z') character = 'A';
                }
                else {
                    cout << number;
                    number++;
                    if (number > 9) number = 0;
                }
                
            } 
            else if ((i + j) % column == column - 1){
                if (!reflex){
                    cout << character;
                    character++;
                    if (character > 'Z') character = 'A';
                }
                else {
                    cout << number;
                    number++;
                    if (number > 9) number = 0;
                }
            } 
            else {
                cout << " ";
            }
            
        }
        cout << endl;
    }

}