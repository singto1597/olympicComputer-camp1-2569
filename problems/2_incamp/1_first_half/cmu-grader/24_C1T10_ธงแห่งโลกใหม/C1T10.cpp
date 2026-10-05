#include <iostream>
using namespace std;
int main() {
    int side;
    cin >> side;
    int number = 0;
    int numberInverse = 0;
    int nowNumber = 0;
    for (int i = 1; i <= side; i++){
        if (i % 2 == 1){
            for (int j = 0; j < i; j++){
                cout << number % 10 << " ";
                number++;
            }
        }
        else{
            for (int j = numberInverse; j >= nowNumber; j--){
                cout << j % 10 << " ";
                number++;
            }
            
        }
        cout << endl;
        nowNumber = number;
        numberInverse = nowNumber + i;
    }

     for (int i = 1; i <= side; i++){
        if (i % 2 == 1){
            for (int j = 1; j <= side; j++){
                if(j < i){
                    cout << "  ";
                    continue;
                }
                if(i == j) number -= j-i+1;
                cout << number % 10 << " ";
                number--;
            }
            //cout << number << " " << endl;
        }
        else{
            for (int j = 1; j <= side; j++){
                if(j < i){
                    cout << "  ";
                    continue;
                }
                if(i == j) number -= side-i;
                cout << number % 10 << " ";
                number++;
                if(j == side) number -= side-i+1;
                //if(j == side) number -= side-i;
            }
            
        }
        cout << endl;
        nowNumber = number;
        numberInverse = nowNumber + i;
    }

    
    
    
    return 0;
    
}