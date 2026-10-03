#include <bits/stdc++.h>
using namespace std;


int main() {
    int n;
    cin >> n;
    int numberOfTable = pow(2,n);
    int grid[numberOfTable][numberOfTable] = {};
    for (int i = 0; i < numberOfTable; i++){
        for (int j = 0; j < numberOfTable; j++){
            cin >> grid[i][j];
        }
    }
    while (numberOfTable > 2){
        for (int i = 0; i < numberOfTable; i+=2){
            for (int j = 0; j < numberOfTable; j+=2){
                int LeftUp = grid[i][j];
                int RightUp = grid[i][j + 1];
                int LeftDown = grid[i + 1][j];
                int RightDown = grid[i + 1][j + 1];

                // LU RU
                // LD RD

                int result = 0;
                if (LeftUp == RightUp && RightUp == LeftDown && LeftDown == RightDown && RightDown == LeftUp){
                    result = LeftUp * 4;
                }
                else{
                    result = max({LeftUp,RightUp,LeftDown,RightDown});
                }
                //cout << result << " ";
                grid[i/2][j/2] = result;

            }  
            //cout << endl;
        }
        numberOfTable /= 2;
    }
    

    vector<int> arrayResult = {grid[0][0], grid[0][1], grid[1][0], grid[1][1]};
    sort(arrayResult.begin(), arrayResult.end());
    for(int i = 0; i < 4; i++){
        cout << arrayResult[i] << " ";
    }
    
    return 0;
    
}