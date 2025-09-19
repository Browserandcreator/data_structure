#include <stdio.h>
#include <stdlib.h>

int k_fibo(int k, int m) {
    if (k < 1 || m < 0) {
        fprintf(stderr, "invalid k or m\n");
        return -1;
    }

    if (m < k - 1) return 0;  
    if (m == k - 1) return 1; 

    // w[0..k-1] 最近k项，w[k]为它们的和S
    int *w = (int*)malloc((k + 1) * sizeof(int)); 
    if (!w) {
        perror("malloc");
        return -1;
    }

    // 初始化k+1项的数组
    for (int i = 0; i < k + 1; ++i)
        // w[k-1]=1, w[k]=1 (S_k=1)
        w[i] = (i < k - 1) ? 0 : 1;

    // 从k + 1移动到m
    for (int i = k + 1; i < m + 1; ++i) {
        // 记录移动时被覆盖的最老项，将每一项左移，并计算新的和项
        int oldest = w[0];                
        for (int j = 0; j < k; ++j) 
            w[j] = w[j + 1];   
        // w[k]+...w[] =                   
        w[k] = 2 * w[k] - oldest;       
    }

    // w的第k项就是第m项
    int ans = w[k];
    free(w);
    return ans;
}


int main(void) {
    int k, m;
    printf("Enter k and m: ");
    scanf("%d %d", &k, &m);
    printf("Fibonacci series: ");
    for (int i = 0; i < m; i++) {
        printf("%d ", k_fibo(k, i));
    }
    return 0;
}

