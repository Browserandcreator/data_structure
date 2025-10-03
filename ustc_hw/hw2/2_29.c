// ABC为三个递增线性表，现在将A中B和C中都有的元素删除，试编写算法实现该功能。
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

void init_list(SeqList* list) {
    list->size = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        list->data[i] = 0;
    }
}

int is_in_list(SeqList* list, int value) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            return 1;
        }
    }
    return 0;
}

void delete_common_elements(SeqList* A, SeqList* B, SeqList* C) {
    for (int i = 0; i < A->size; i++) {
        if (is_in_list(B, A->data[i]) && is_in_list(C, A->data[i])) {
            // 删除A中与B和C中都有的元素
            for (int j = i; j < A->size - 1; j++) {
                A->data[j] = A->data[j + 1];
            }
            A->size--;
            i--;  // 重新检查当前位置
        }
    }
}

// 假设 SeqList { int *data; int size; } 且 data 已按升序
void delete_common_elements_optimized(SeqList* A, SeqList* B, SeqList* C) {
    int i = 0, j = 0, k = 0;   // 读指针：A、B、C
    int w = 0;                 // 写指针：A 的新末尾

    while (i < A->size) {
        int x = A->data[i];

        // 将 j、k 前推到 >= x 的位置（利用有序性）
        while (j < B->size && B->data[j] < x) j++;
        while (k < C->size && C->data[k] < x) k++;

        // 检查 x 是否同时出现在 B 和 C
        int inB = (j < B->size && B->data[j] == x);
        int inC = (k < C->size && C->data[k] == x);

        if (inB && inC) {
            // 跳过：相当于删除 x
            i++;
        } else {
            // 保留：覆写到 A[w]
            if (w != i) A->data[w] = x;
            w++; i++;
        }
    }
    A->size = w;
}


void print_list(SeqList* list) {
    for (int i = 0; i < list->size; i++) {
        printf("%d -> ", list->data[i]);
    }
    printf("NULL\n");
}

int main() {
    SeqList A, B, C;
    init_list(&A);
    init_list(&B);
    init_list(&C);

    // 初始化A, B, C
    A.data[0] = 1; A.data[1] = 2; A.data[2] = 3; A.data[3] = 4; A.size = 4;
    B.data[0] = 2; B.data[1] = 3; B.size = 2;
    C.data[0] = 3; C.data[1] = 4; C.size = 2;

    printf("Original A: ");
    print_list(&A);
    printf("\n");

    printf("B: ");
    print_list(&B);
    printf("\n");

    printf("C: ");
    print_list(&C);
    printf("\n");

    delete_common_elements_optimized(&A, &B, &C);

    printf("Modified A: ");
    print_list(&A);
    printf("\n");

    return 0;
}