#include <bits/stdc++.h>
using namespace std;

int main (){
    int n;
    cin >> n;

    for (int i = 0; i < n; i++){
        int numberOfString = 0;
        cin >> numberOfString;
        string messageWord[numberOfString];
        for (int j = 0; j < numberOfString; j++){
            cin >> messageWord[j];
        }
        int countOfDifferents = 0;
        bool isFalse = false;

        //ตรวจสอบ
        for (int j = 0; j < numberOfString - 1; j++){
            countOfDifferents = 0;

            //ถ้ามันยาวมากกว่า 1
            if (1 < abs(static_cast<int>(messageWord[j].length()) - static_cast<int>(messageWord[j+1].length()))){
                isFalse = true;
            }

            //ถ้ายาวเท่ากัน
            else if (messageWord[j].length() == messageWord[j+1].length()){
                //ดักควาย
                if (messageWord[j] == messageWord[j+1]){
                    isFalse = true;
                    continue;
                }

                //ตรวจสอบว่ามันเหมือนกันมั้ย
                for (int indexOfStr = 0; indexOfStr < min(messageWord[j].length(),messageWord[j+1].length()); indexOfStr++){
                    if (messageWord[j][indexOfStr] != messageWord[j + 1][indexOfStr] ){
                        countOfDifferents++;
                    }
                    
                }
                if (countOfDifferents > 1){
                    isFalse = true;
                }
            }

            //ถ้ายาวต่างกัน 1 ตัว
            else{
                string longWord, shortWord;

                if (messageWord[j].length() > messageWord[j+1].length()){
                    longWord = messageWord[j];
                    shortWord = messageWord[j+1];
                }
                else{
                    longWord = messageWord[j+1];
                    shortWord = messageWord[j];
                }

                //เอาไว้นับ
                int indexOfStr = 0;

                //เอาไว้บ่งบอกตำแหน่งของข้อความ
                char *longWordIndexPoiter = &longWord[0];
                char *shortWordIndexPoiter = &shortWord[0];

                while(indexOfStr < longWord.length()){
                    if (*longWordIndexPoiter == *shortWordIndexPoiter){
                        //ไม่เกิดอะไรขึ้น เลื่อนไป 1 Index
                        longWordIndexPoiter++;
                        shortWordIndexPoiter++;
                    }

                    else if (*longWordIndexPoiter != *shortWordIndexPoiter){
                        //เลื่อนไปเช็ค 1 Index แค่ตัวยาว 
                        longWordIndexPoiter++;
                    }

                    if (*longWordIndexPoiter != *shortWordIndexPoiter){
                        countOfDifferents++;
                    }

                    if (countOfDifferents > 1){
                        isFalse = true;
                    }
                    indexOfStr++;
                }
            }
        }


        
        if (isFalse){
            cout << "F" << endl;
        }
        else{
            cout << "T" << endl;
        }
    }

}