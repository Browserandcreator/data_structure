// 实现一个链表删除函数，高效删除链表中值在mink和maxk之间的节点
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

// 已知链表按顺序排列，高效删除值在[mink, maxk]区间的节点
struct Node* delete_range(struct Node* head, int mink, int maxk) {
    while (head != NULL && head->data > mink && head->data < maxk) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }

    if (head == NULL) return NULL;

    struct Node* curr = head;
    while (curr != NULL && curr->next != NULL) {
        if (curr->next->data > mink && curr->next->data < maxk) {
            struct Node* temp = curr->next;
            curr->next = curr->next->next;
            free(temp);
        } else {
            curr = curr->next;
        }
    }
    return head;
}

void print_list(struct Node* head) {
    struct Node* curr = head;
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

int main(void) {
    // 创建一个示例链表: 1 -> 3 -> 5 -> 7 -> 9
    struct Node* head = create_node(1);
    head = add_node(head, 2);
    head = add_node(head, 3);
    head = add_node(head, 4);
    head = add_node(head, 5);
    head = add_node(head, 6);
    head = add_node(head, 7);
    head = add_node(head, 8);
    head = add_node(head, 9);

    printf("Original list: \n");
    print_list(head);

    int mink = 4, maxk = 8;
    head = delete_range(head, mink, maxk);

    printf("List after deleting nodes with values in range [%d, %d]: \n", mink, maxk);
    print_list(head);

    // 释放剩余节点
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}