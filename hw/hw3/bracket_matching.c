#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ========== 顺序表（题目给定：表达式字符已存在顺序表中） ========== */
typedef struct {
    const char *data;  // 指向一段只读字符
    int length;        // 长度
} SqList;

static inline SqList make_sq(const char *s) {
    SqList L;
    L.data = s;
    L.length = (int)strlen(s);
    return L;
}

/* ========== 简单顺序栈 ========== */
typedef struct {
    char *a;
    int top, cap;
} Stack;

static void st_init(Stack *st, int cap) {
    st->a = (char *)malloc((cap > 0 ? cap : 1) * sizeof(char));
    st->top = -1;
    st->cap = (cap > 0 ? cap : 1);
}
static bool st_empty(const Stack *st) { return st->top == -1; }
static void st_push(Stack *st, char x) {
    if (st->top + 1 >= st->cap) {                // 扩容
        st->cap = st->cap * 2;
        st->a = (char *)realloc(st->a, st->cap * sizeof(char));
    }
    st->a[++st->top] = x;
}
static char st_pop(Stack *st) { return st->a[st->top--]; }
static char st_peek(const Stack *st) { return st->a[st->top]; }
static void st_destroy(Stack *st) {
    free(st->a);
    st->a = NULL;
    st->top = -1;
    st->cap = 0;
}

/* ========== 工具 ========== */
static inline bool isLeft(char c){ return c=='('||c=='['||c=='{'; }
static inline bool isRight(char c){ return c==')'||c==']'||c=='}'; }
static inline bool match(char L, char R){
    return (L=='('&&R==')') || (L=='['&&R==']') || (L=='{'&&R=='}');
}

/* ========== 判定函数：只检查三种括号 () [] {} ========== */
bool BracketsMatched(SqList S){
    Stack st; st_init(&st, S.length);
    for (int i = 0; i < S.length; ++i) {
        char c = S.data[i];
        if (isLeft(c)) {
            st_push(&st, c);
        } else if (isRight(c)) {
            if (st_empty(&st) || !match(st_peek(&st), c)) {
                st_destroy(&st);
                return false;
            }
            (void)st_pop(&st);
        } // 其他字符忽略
    }
    bool ok = st_empty(&st);
    st_destroy(&st);
    return ok;
}

/* ========== 测试 ========== */
int main(void) {
    const char *tests[] = {
        "([{}])",              // ✅
        "([]{})",              // ✅
        "([}{])",              // ❌
        "([)]",                // ❌
        "([]",                 // ❌
        "abc{[()]}123",        // ✅ (忽略非括号字符)
        "",                    // ✅ 空串
        "{[()]}[]{}",          // ✅
        "{[(])}",              // ❌
        "(((([[]]))){})"       // ✅
    };
    int n = (int)(sizeof(tests)/sizeof(tests[0]));

    for (int i = 0; i < n; ++i) {
        SqList L = make_sq(tests[i]);
        bool ok = BracketsMatched(L);
        printf("[%2d] %-20s -> %s\n", i+1, tests[i], ok ? "匹配正确" : "不匹配");
    }
    return 0;
}
