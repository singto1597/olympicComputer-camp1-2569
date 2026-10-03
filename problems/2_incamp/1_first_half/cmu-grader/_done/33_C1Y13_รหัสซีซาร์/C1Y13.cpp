#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    string passwordSecret;
    getline(cin, passwordSecret);
    int n;
    cin >> n;
    string password = "";
    for (int i = 0; i < passwordSecret.length(); i++){
        int asciiBuffer = (int)passwordSecret[i];
        char buffer;
        if (passwordSecret[i] == ' '){
            password = password + ' ' ;
        }
        else{
            
            if (asciiBuffer >= 'a' && asciiBuffer <= 'z'){
                buffer = 'a' + ((asciiBuffer - 'a' - n + 26) % 26);
            }
            else if (asciiBuffer >= 'a' && asciiBuffer <= 'z') {
            buffer = 'a' + ((asciiBuffer - 'a' - n + 26) % 26);
        } 
        else if (asciiBuffer >= 'A' && asciiBuffer <= 'Z') {
            buffer = 'A' + ((asciiBuffer - 'A' - n + 26) % 26);
        } 
        else {
            buffer = asciiBuffer;
        }
            password += buffer;
            
        }
        
    }
    cout << password << endl;

    return 0;
    
}