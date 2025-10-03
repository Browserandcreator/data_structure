/*
 * Ackermann 函数（题 3.27⑤）
 * - 递归算法: akm_rec
 * - 非递归算法: akm_iter
 * - 非递归算法带轨迹打印: akm_iter_trace
 * - 递归算法带轨迹打印: akm_rec_trace
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*==================== 递归版本 ====================*/
unsigned long long akm_rec(unsigned int m, unsigned int n) {
    if (m == 0) return n + 1ULL;
    if (n == 0) return akm_rec(m - 1, 1);
    return akm_rec(m - 1, akm_rec(m, n - 1));
}

/*==================== 简易整型栈 ====================*/
typedef struct {
    int *a;
    int top, cap;
} IntStack;

static void st_init(IntStack *s, int cap) {
    if (cap <= 0) cap = 4;
    s->a = (int*)malloc(sizeof(int)*cap);
    s->cap = cap;
    s->top = -1;
}
static bool st_empty(const IntStack *s){ return s->top < 0; }
static void st_push(IntStack *s, int x){
    if (s->top + 1 >= s->cap){
        s->cap *= 2;
        s->a = (int*)realloc(s->a, sizeof(int)*s->cap);
    }
    s->a[++s->top] = x;
}
static int st_pop(IntStack *s){ return s->a[s->top--]; }
static void st_free(IntStack *s){ free(s->a); s->a=NULL; s->cap=s->top=0; }

/*==================== 非递归版本 ====================*/
unsigned long long akm_iter(unsigned int m, unsigned int n) {
    IntStack s; st_init(&s, 16);
    for(;;){
        if (m == 0){
            n = n + 1;
            if (st_empty(&s)){
                st_free(&s);
                return n;
            }
            m = (unsigned)st_pop(&s);
        } else if (n == 0){
            m = m - 1;
            n = 1;
        } else {
            st_push(&s, (int)(m - 1));
            n = n - 1;
        }
    }
}

/*==================== 非递归版本打印轨迹 ====================*/
void akm_iter_trace(unsigned int m, unsigned int n) {
    IntStack s; st_init(&s, 16);
    unsigned step = 0;

    printf("Trace for akm_iter(%u,%u):\n", m, n);
    for(;;){
        printf("step %2u: m=%u, n=%u, stack=[", step++, m, n);
        for (int i = 0; i <= s.top; ++i){
            printf("%d", s.a[i]);
            if (i < s.top) printf(", ");
        }
        printf("]\n");

        if (m == 0){
            n = n + 1;
            printf("m==0 -> n=n+1 => n=%u\n", n);
            if (st_empty(&s)){
                printf("stack empty -> result = %u\n", n);
                break;
            }
            m = (unsigned)st_pop(&s);
            printf("pop -> resume outer with m=%u\n", m);
        } else if (n == 0){
            m = m - 1;
            n = 1;
            printf("n==0 -> (m,n)=(%u,%u)\n", m, n);
        } else {
            st_push(&s, (int)(m - 1));
            n = n - 1;
            printf("push(m-1) & n-- -> push %u, now n=%u\n", m-1, n);
        }
    }
    st_free(&s);
}

/*==================== 递归版本打印轨迹 ====================*/
static void indent(int d){ while(d--) putchar(' '); }
static unsigned long long step_id = 0;

unsigned long long akm_rec_trace(unsigned m, unsigned n, int depth){
    indent(depth); printf("step %llu: call  akm(%u,%u)\n", ++step_id, m, n);

    if(m == 0){
        unsigned long long r = n + 1ULL;
        indent(depth); printf("ret   akm(%u,%u) = %llu  (m==0)\n", m, n, r);
        return r;
    }
    if(n == 0){
        indent(depth); printf("-> reduce to akm(%u,%u)\n", m-1, 1);
        unsigned long long r = akm_rec_trace(m-1, 1, depth+2);
        indent(depth); printf("ret   akm(%u,%u) = %llu\n", m, n, r);
        return r;
    }

    indent(depth); printf("-> compute inner t = akm(%u,%u)\n", m, n-1);
    unsigned long long t = akm_rec_trace(m, n-1, depth+2);
    indent(depth); printf("-> then compute akm(%u,%llu)\n", m-1, t);
    unsigned long long r = akm_rec_trace(m-1, (unsigned)t, depth+2);

    indent(depth); printf("ret   akm(%u,%u) = %llu\n", m, n, r);
    return r;
}

/*==================== 测试 ====================*/
int main(void){
    unsigned int m = 2, n = 1;
    unsigned long long r1 = akm_rec(m, n);
    unsigned long long r2 = akm_iter(m, n);
    printf("akm_rec(%u,%u)  = %llu\n", m, n, r1);
    printf("akm_iter(%u,%u) = %llu\n\n", m, n, r2);

    /* 非递归求值过程 */
    akm_iter_trace(2, 1);

    /* 递归求值过程 */
    printf("\nTrace for akm_rec(%u,%u):\n", m, n);
    step_id = 0;
    unsigned long long r3 = akm_rec_trace(m, n, 0);
    printf("\nResult from akm_rec_trace: akm(%u,%u) = %llu\n", m, n, r3);

    return 0;
}
