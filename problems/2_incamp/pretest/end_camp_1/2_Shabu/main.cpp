#include <bits/stdc++.h>

using namespace std;

struct Menu{
    string name;
    char type; // M Meat | V Veget
    int time;
    int price;
    int stock;
};

struct Customer{
    int customers_id;
    int time_i;
    int bill;
    int amount;
    string name;
};

bool comp(Customer &a, Customer &b){
    if (a.bill != b.bill){
        return a.bill < b.bill;
    }
    else if (a.time_i != b.time_i){
        return a.time_i < b.time_i;
    }
    else if (a.customers_id != b.customers_id){
        return a.customers_id < b.customers_id;
    }
    else if (a.amount != b.amount){
        return a.amount < b.amount;
    }
    else {
        a.name < b.name;
    }
}

int main(){
    int hour, minute;
    char c;
    cin >> hour >> c >> minute;
    int time_start = hour * 60 + minute;
    // cout << hour << c << minute << endl;

    int time_max, l, j;
    cin >> time_max >> l >> j;

    int m;
    cin >> m;


    vector<Menu> menus(m + 5);

    for (int j = 1; j <= m; j++){
        string name;
        char type; // M Meat | V Veget
        int time;
        int price;
        int stock;
        cin >> name >> type >> time >> price >> stock;
        menus[j] = {name, type, time, price, stock};
        
    }

    int n;
    cin >> n;

    vector<Customer> customers(n + 5);

    for (int i = 1; i <= n; i++){
        cin >> hour >> c >> minute;
        int time_i = hour * 60 + minute;
        int bill;
        int amount;
        string name;
        cin >> time_i >> bill >> amount >> name;
        customers[i] = {i, time_i, bill, amount, name};
    }

    sort(customers.begin(), customers.end(), comp);


    for (int i = 1; i <= n; i++){
        int customers_id = customers[i].customers_id;
        int time_i = customers[i].time_i;
        int bill = customers[i].bill;
        int amount = customers[i].amount;
        string name = customers[i].name;
        cout << customers_id << " ";
        cout << time_i << " ";
        cout << bill << " ";
        cout << amount << " ";
        cout << name << endl;
    }






}