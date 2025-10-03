// 按照访问频次排序的双向链表
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct DNode {
    int data;
    int freq;
    struct DNode *prev, *next;
} DNode, *DList;

/* 建立带头结点的双向循环链表 */
DList InitList(void) {
    DList L = (DList)malloc(sizeof(DNode));
    L->data = 0;
    L->freq = INT_MAX;              // 头结点放一个极大频度，便于统一处理
    L->prev = L->next = L;
    return L;
}

/* 把节点 p 从原位置摘下 */
static inline void detach(DNode *p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
}

/* 在结点 q 之后插入结点 p：... q <-> p <-> q->next ... */
static inline void insert_after(DNode *q, DNode *p) {
    p->next = q->next;
    p->prev = q;
    q->next->prev = p;
    q->next = p;
}

/* LOCATE(L, x)：找到值为 x 的结点，频度+1，并按非增序稳定重排 */
DNode* LOCATE(DList L, int x) {
    DNode *p = L->next;
    while (p != L && p->data != x) p = p->next;
    if (p == L) return NULL;        // 不存在

    p->freq++;                      // 访问次数+1

    /* 若 p 已经在正确位置（前驱频度 >= p->freq 且后继频度 <= p->freq），无需移动。
       这里只检查向前是否需要移动即可。*/
    if (p->prev->freq >= p->freq) return p;

    /* 从 p 的前驱往前找第一个 freq >= p->freq 的结点 q */
    DNode *q = p->prev;
    while (q != L && q->freq < p->freq) q = q->prev;

    /* 将 p 移到 q 之后（稳定：遇到相等就停，所以同频不越过旧元素） */
    detach(p);
    insert_after(q, p);

    return p;
}

/* INSERT(L, x)：插入值为 x 的新结点，频度置1，并按非增序稳定重排 */
DNode* INSERT(DList L, int x) {
    DNode *p = (DNode*)malloc(sizeof(DNode));
    p->data = x;
    p->freq = 1;

    /* 从表头开始找第一个 freq < 1 的结点 q（即第一个结点） */
    DNode *q = L->next;
    while (q != L && q->freq >= p->freq) q = q->next;

    /* 将 p 插入到 q 之前 */
    insert_after(q->prev, p);

    return p;
}

/* PRINT(L)：打印链表 */
void PRINT(DList L) {
    DNode *p = L->next;
    while (p != L) {
        printf("(%d, %d) ", p->data, p->freq);
        p = p->next;
    }
    printf("\n");
}

/* 销毁链表 */
void DestroyList(DList L) {
    DNode *p = L->next, *q;
    while (p != L) {
        q = p->next;
        free(p);
        p = q;
    }
    free(L);
}

int main(void) {
    DList L = InitList();

    INSERT(L, 5);
    INSERT(L, 3);
    INSERT(L, 8);
    INSERT(L, 6);
    PRINT(L);                   

    LOCATE(L, 5);
    PRINT(L);                   

    LOCATE(L, 8);
    PRINT(L);                   

    LOCATE(L, 8);
    PRINT(L);                   

    LOCATE(L, 3);
    LOCATE(L, 3);  
    LOCATE(L, 3);
    PRINT(L);   

    LOCATE(L, 5);
    PRINT(L);     
    
    LOCATE(L, 6);
    LOCATE(L, 6);
    LOCATE(L, 6);
    LOCATE(L, 6);
    LOCATE(L, 6);
    LOCATE(L, 6);
    PRINT(L);

    DestroyList(L);
    return 0;
}