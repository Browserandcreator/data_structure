#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 插入排序，升序
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        // i从1开始，因为一开始手上只有一个元素，本来就是有序的
        // 每次排序 arr[i]，而arr[0..i-1] 已经是有序的
        int key = arr[i];
        int j = i - 1;
        // 将 key 插入到已排序部分 arr[0..i-1]
        // 特别注意边界：至多和 i 个元素比较，即 j = i-1 到 j = 0
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

int main() {
    vector<int> arr = {5, 2, 9, 1, 5, 6};
    insertionSort(arr);
    cout << "排序后的数组: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
    return 0;
}