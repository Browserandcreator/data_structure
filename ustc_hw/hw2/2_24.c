// 实现两个递增链表的合并，合并后链表变为递减，不开辟两链表之外的任何空间
#include <stdio.h>
#include <stdlib.h>
struct Node {
    int data;
    struct Node* next;
};

struct Node* create_node(int data) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    if (!new_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}
// 尾部增加节点
struct Node* add_node(struct Node* head, int data) {
    struct Node* new_node = create_node(data);
    if (!head) {
        return new_node;
    }
    struct Node* curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
    return head;

}
// 合并两个递增链表，合并后链表变为递减
struct Node* merge_and_reverse(struct Node* l1, struct Node* l2) {
    struct Node* merged = NULL;
    while (l1 != NULL && l2 != NULL) {
        if (l1->data < l2->data) {
            struct Node* next = l1->next;
            l1->next = merged;
            merged = l1;
            l1 = next;
        } else {
            struct Node* next = l2->next;
            l2->next = merged;
            merged = l2;
            l2 = next;
        }
    }
    while (l1 != NULL) {
        struct Node* next = l1->next;
        l1->next = merged;
        merged = l1;
        l1 = next;
    }
    while (l2 != NULL) {
        struct Node* next = l2->next;
        l2->next = merged;
        merged = l2;
        l2 = next;
    }
    return merged;
}

void print_list(struct Node* head) {
    struct Node* curr = head;
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

int main() {
    struct Node* l1 = NULL;
    struct Node* l2 = NULL;

    // 创建第一个递增链表
    l1 = add_node(l1, 1);
    l1 = add_node(l1, 3);
    l1 = add_node(l1, 5);

    // 创建第二个递增链表
    l2 = add_node(l2, 2);
    l2 = add_node(l2, 4);
    l2 = add_node(l2, 6);

    printf("List 1: ");
    print_list(l1);
    printf("List 2: ");
    print_list(l2);

    struct Node* merged = merge_and_reverse(l1, l2);
    printf("Merged and Reversed List: ");
    print_list(merged);

    // 释放内存
    struct Node* curr = merged;
    while (curr != NULL) {
        struct Node* temp = curr;
        curr = curr->next;
        free(temp);
    }

    return 0;
}

