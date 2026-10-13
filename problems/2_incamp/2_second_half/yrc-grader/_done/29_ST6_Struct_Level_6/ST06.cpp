#include <bits/stdc++.h>
using namespace std;

struct Workshop{
    int id;
    long long start;
    long long finish;
};

Workshop workshop[200005];

bool comp(Workshop &a, Workshop &b){
    if (a.finish != b.finish) return a.finish < b.finish;
    else if (a.id != b.id) return a.id < b.id;
    else return a.start >= a.finish;
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++){
        cin >> workshop[i].id;
        cin >> workshop[i].start;
        cin >> workshop[i].finish;
    }

    sort(workshop, workshop + n, comp);

    int count = 0;

    long long last_finisted = workshop[0].finish;
    count++;
    vector<int> selected;
    selected.push_back(workshop[0].id);

    for (int i = 0; i < n; i++){
        // cout << workshop[i].id;
        // cout << " ";
        // cout << workshop[i].start;
        // cout << " ";
        // cout << workshop[i].finish;
        // cout << endl;
        if (last_finisted <= workshop[i].start){
            count++;
            last_finisted = workshop[i].finish;
            selected.push_back(workshop[i].id);
        }
    }

    cout << count << endl;
    for (int x : selected){
        cout << x << " ";
    }
    return 0;

}

