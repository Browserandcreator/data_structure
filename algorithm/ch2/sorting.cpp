#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

// 排序函数声明
void bubbleSort(vector<int>& arr);    // TODO: 实现冒泡排序
void selectionSort(vector<int>& arr); // TODO: 实现选择排序
void insertionSort(vector<int>& arr); // TODO: 实现插入排序
void quickSort(vector<int>& arr);     // TODO: 实现快速排序
void mergeSort(vector<int>& arr);     // TODO: 实现归并排序

// 计时工具
template<typename Func>
double measureSort(Func sortFunc, vector<int> arr) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> diff = end - start;
    return diff.count();
}

int main() {
    random_device rd;
    mt19937 gen(rd());

    while (true) {
        int n;
        cout << "请输入数组长度n（输入0退出）：";
        cin >> n;
        if (n <= 0) break;

        // 生成随机排列
        vector<int> arr(n);
        for (int i = 0; i < n; ++i) arr[i] = i + 1;
        shuffle(arr.begin(), arr.end(), gen);

        // 复制多份用于不同排序
        vector<int> arr_bubble = arr;
        vector<int> arr_selection = arr;
        vector<int> arr_insertion = arr;
        vector<int> arr_quick = arr;
        vector<int> arr_merge = arr;

        // 计时并排序
        double t_bubble = measureSort(bubbleSort, arr_bubble);
        double t_selection = measureSort(selectionSort, arr_selection);
        double t_insertion = measureSort(insertionSort, arr_insertion);
        double t_quick = measureSort(quickSort, arr_quick);
        double t_merge = measureSort(mergeSort, arr_merge);

        // 输出结果
        cout << "排序完成，耗时（毫秒）：" << endl;
        cout << "冒泡排序:   " << t_bubble << endl;
        cout << "选择排序:   " << t_selection << endl;
        cout << "插入排序:   " << t_insertion << endl;
        cout << "快速排序:   " << t_quick << endl;
        cout << "归并排序:   " << t_merge << endl;
        cout << "-----------------------------" << endl;
    }
    return 0;
}

// TODO: 实现各排序算法
void bubbleSort(vector<int>& arr) {
    // TODO
}
void selectionSort(vector<int>& arr) {
    // TODO
}
void insertionSort(vector<int>& arr) {
    // TODO
}
void quickSort(vector<int>& arr) {
    // TODO
}
void mergeSort(vector<int>& arr) {
    // TODO
}