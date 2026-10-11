#include <bits/stdc++.h>

using namespace std;

int lengthOfString;
string result;
string baseInput;
set<string> setOfInfo;

void findSubString(int number){
    if (number == lengthOfString){
        bool isFalse = false;
        
        if (!result.empty()){
            int indexStarts = -1;

            string buffer = result;
            result = "";

            if (buffer == ""){
                return;
            } 
            for (int i = 0; i < buffer.size(); i++){
                indexStarts = i;
                if (buffer[i] != '0'){
                    break;
                } 
            }

            if (indexStarts == buffer.size() - 1){
                result = "0";
            }
            else{
                for (int i = indexStarts; i < buffer.size(); i++){
                    result = result + buffer[i];
                }
                
            }


            for (int i = 0; i < result.size(); i++){
                if (i % 2 == 0){
                    if (result[i] == '0'){
                        isFalse = true;
                    }
                    
                }
                else {
                    if (result[i] == '1'){
                        isFalse = true;
                    }
                }
            }
        }
        if (!isFalse && !result.empty()){
            setOfInfo.insert(result);
        }
        
    }
    else{
        if (baseInput == ""){
            
        }
        result.push_back(baseInput[number]);
        findSubString(number+1);

        result.pop_back();

        findSubString(number+1);
    }
}

int main(){
    cin >> lengthOfString;
    cin >> baseInput;

    findSubString(0);

    for (auto i : setOfInfo){
        cout << i << endl;
    }

    cout << setOfInfo.size() << endl;
}