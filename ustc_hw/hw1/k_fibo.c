#include <stdio.h>
#include <stdlib.h>

// 计算 k 阶斐波那契数列的第 m 项
long long kFibo(int k, int m) {
    if (k < 1 || m < 0) {
        fprintf(stderr, "invalid input\n");
        return -1;
    }

    if (m < k - 1) return 0;
    if (m == k - 1) return 1;

    // w[0]...w[k]
    long long *w = (long long *)calloc(k, sizeof(long long));
    if (!w) {
        perror("calloc");
        return -1;
    }

    w[k - 1] = 1;  

    long long next = 0;
    for (int i = k; i <= m; i++) {
        next = 0;
        for (int j = 0; j < k; j++) next += w[j];

        // 左移窗口
        for (int j = 0; j < k - 1; j++) w[j] = w[j + 1];
        w[k - 1] = next;  // 新项放到最后
    }

    free(w);
    return next;  // 返回 f_m
}

int main() {
    int k, m;
    printf("Enter k and m: ");
    scanf("%d %d", &k, &m);
    for (int i = 0; i < m; i++) {
        printf("%d ", kFibo(k, i));
    }
    return 0;
}
