//对递增线性表A，B，求两者交集C，C也保持递增有序
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAXSIZE 20

typedef int ElemType;
typedef struct
{
    ElemType data[MAXSIZE];
    int length;
} SqList;

void InitList(SqList *L)
{
    L->length = 0;
}



bool ListInsert(SqList *L, int i, ElemType e)
{
    if (i < 1 || i > L->length + 1)
        return false;
    if (L->length >= MAXSIZE)
        return false;
    for (int j = L->length; j >= i; j--)
        L->data[j] = L->data[j - 1];
    L->data[i - 1] = e;
    L->length++;
    return true;
}

bool ListDelete(SqList *L, int i, ElemType *e)
{
    if (i < 1 || i > L->length)
        return false;
    *e = L->data[i - 1];
    for (int j = i; j < L->length; j++)
        L->data[j - 1] = L->data[j];
    L->length--;
    return true;
}

void CreateList(SqList *L, ElemType a[], int n)
{
    for (int i = 0; i < n; i++)
        ListInsert(L, i + 1, a[i]);
}

void PrintList(SqList L)
{
    for (int i = 0; i < L.length; i++)
        printf("%d ", L.data[i]);
    printf("\n");
}

void Intersection(SqList A, SqList B, SqList *C)
{
    InitList(C);
    int i = 0, j = 0;
    while (i < A.length && j < B.length)
    {
        if (A.data[i] < B.data[j])
            i++;
        else if (A.data[i] > B.data[j])
            j++;
        else
        {
            ListInsert(C, C->length + 1, A.data[i]);
            i++;
            j++;
        }
    }
}

int main()
{
    SqList A, B, C;
    InitList(&A);
    InitList(&B);
    int a[] = {1, 2, 4, 5, 6};
    int b[] = {2, 3, 5, 7};
    CreateList(&A, a, 5);
    CreateList(&B, b, 4);
    printf("List A: ");
    PrintList(A);
    printf("List B: ");
    PrintList(B);
    Intersection(A, B, &C);
    printf("Intersection C: ");
    PrintList(C);
    return 0;
}