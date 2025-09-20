// 实现一个顺序表的逆转
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

// 链表逆转
void reverse_seq_list(SeqList* list) {
    int left = 0;
    int right = list->size - 1;
    while (left < right) {
        int temp = list->data[left];
        list->data[left] = list->data[right];
        list->data[right] = temp;
        left++;
        right--;
    }
}

// 数组逆转
void reverse_array(int* arr, int size) {
    int left = 0;
    int right = size - 1;
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

void print_seq_list(const SeqList* list) {
    for (int i = 0; i < list->size; i++) {
        printf("%d -> ", list->data[i]);
    }
    printf("NULL\n");
}

void print_array(const int* arr, int size) {
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    SeqList list = {{1, 2, 3, 4, 5}, 5};
    int arr[] = {1, 2, 3, 4, 5};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    printf("Original SeqList: ");
    print_seq_list(&list);
    reverse_seq_list(&list);
    printf("Reversed SeqList: ");
    print_seq_list(&list);

    printf("Original Array: ");
    print_array(arr, arr_size);
    reverse_array(arr, arr_size);
    printf("Reversed Array: ");
    print_array(arr, arr_size);

    return 0;
}