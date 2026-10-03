#include <iostream>
using namespace std;
int main() {
    double Midterm_Score, Final_Score, HomeworkScore;
    cin >> Midterm_Score >> Final_Score >> HomeworkScore;
    double CSubjectScore = Midterm_Score * 0.4 + Final_Score * 0.5 + HomeworkScore * 0.1;
    if (CSubjectScore >= 80 && CSubjectScore <= 100) cout << "G" << endl;
    else if (CSubjectScore >= 50 && CSubjectScore <= 79) cout << "P" << endl;
    else cout << "F" << endl;
    
    return 0;
    
}