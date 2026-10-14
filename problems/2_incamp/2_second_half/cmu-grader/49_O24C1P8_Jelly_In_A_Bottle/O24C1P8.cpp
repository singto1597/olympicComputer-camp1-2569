#include <bits/stdc++.h>

using namespace std;

struct Jelly{
    double d;
    double v;
};

void print_jelly(deque<Jelly> dq){
    while(!dq.empty()){
        Jelly current = dq.front();
        dq.pop_front();
        cout << "(";
        cout << fixed << setprecision(2) << current.d;
        cout << ", ";
        cout << " ";
        cout << fixed << setprecision(2) << current.v;
        cout << ")";
        cout << " ";
    }
    cout << endl;
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int q;
    cin >> q;

    deque<Jelly> dq;

    while (q--){
        int choise;
        cin >> choise;

        if (choise == 1){
            print_jelly(dq);
            continue;
        }

        double d;
        double v;
        cin >> d >> v;



        while (!dq.empty() && dq.back().d <= d){
            Jelly top = dq.back();
            dq.pop_back();
            double top_d = top.d;
            double top_v = top.v;

            double v_new = v + top_v;
            double d_new = ((double)v * d + (double)top_v * top_d) / (double)v_new;
            d = d_new;
            v = v_new;
        }


        dq.push_back({d, v});
    }

}
