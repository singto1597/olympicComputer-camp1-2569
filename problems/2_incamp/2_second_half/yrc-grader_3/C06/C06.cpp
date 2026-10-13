#include <bits/stdc++.h>

using namespace std;

long long count_a = 0;

void merge(vector<long long> &arr, int left, int mid, int right){
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> left_arr(n1), right_arr(n2);

    for (int i = 0; i < n1; i++) left_arr[i] = arr[left + i];
    for (int i = 0; i < n2; i++) right_arr[i] = arr[mid + i + 1];
    int i = 0, j = 0, k = left;

    while(i < n1 && j < n2){
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        }
        else {
            arr[k] = right_arr[j];
            count_a+=n1-i;
            j++;
        }
        k++;
    }
    while (i < n1) { arr[k] = left_arr[i]; i++; k++; }
    while (j < n2) { arr[k] = right_arr[j]; j++; k++; }

}

void mergeSort(vector<long long>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}



int main(){
    long long n;
    cin >> n;
    vector<long long> arr;
    for (int i = 0; i < n; i++){
        long long x;
        cin >> x;
        arr.push_back(x);
    }



    // if (n <= 3000){

    //     for (int i = 0; i < n; i++){
    //         for (int j = 0; j < n - 1; j++){
    //             if (arr[j] > arr[j + 1]){
    //                 swap(arr[j], arr[j + 1]);
    //                 count_a ++;
    //             }
    //         }
    //     }
    // }
    // else{
        mergeSort(arr, 0, arr.size() - 1);
        //     for (int i = 0; i < n; i++){
        //     cout << arr[i] << " ";
        // }


    // }


    cout << count_a << endl;
}
