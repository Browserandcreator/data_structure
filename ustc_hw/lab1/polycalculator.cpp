#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <climits>
using namespace std;

struct Node {
    long long c;  // 系数
    int e;        // 指数
    Node* next;
    Node(long long c_, int e_, Node* nx=nullptr): c(c_), e(e_), next(nx) {}
};

class Poly {
public:
    Poly() { head = new Node(0, INT_MAX); }
    Poly(const Poly& other) { head = new Node(0, INT_MAX); copyFrom(other); }
    Poly& operator=(const Poly& other){ if(this!=&other){clear(); copyFrom(other);} return *this; }
    Poly(Poly&& other) noexcept { head = other.head; other.head = new Node(0, INT_MAX); }
    Poly& operator=(Poly&& other) noexcept {
        if (this != &other) { destroy(); head = other.head; other.head = new Node(0, INT_MAX); }
        return *this;
    }
    ~Poly(){ destroy(); }

    void insertTerm(long long c, int e) {
        if (c == 0) return;
        Node* prev = head; Node* cur = head->next;
        while (cur && cur->e > e) { prev = cur; cur = cur->next; }
        if (cur && cur->e == e) {
            cur->c += c;
            if (cur->c == 0) { prev->next = cur->next; delete cur; }
        } else {
            prev->next = new Node(c, e, cur);
        }
    }
    void buildFromPairs(const vector<pair<long long,int>>& terms){
        for (auto &t: terms) insertTerm(t.first, t.second);
    }

    Poly add(const Poly& B) const {
        Poly R; Node *p=head->next, *q=B.head->next, *r=R.head;
        while (p||q){
            if (q==nullptr || (p&&p->e>q->e)) { r->next=new Node(p->c,p->e); r=r->next; p=p->next; }
            else if (p==nullptr || (q&&q->e>p->e)) { r->next=new Node(q->c,q->e); r=r->next; q=q->next; }
            else { long long c=p->c+q->c; if(c) { r->next=new Node(c,p->e); r=r->next; } p=p->next; q=q->next; }
        }
        return R;
    }
    Poly sub(const Poly& B) const {
        Poly R; Node *p=head->next, *q=B.head->next, *r=R.head;
        while (p||q){
            if (q==nullptr || (p&&p->e>q->e)) { r->next=new Node(p->c,p->e); r=r->next; p=p->next; }
            else if (p==nullptr || (q&&q->e>p->e)) { r->next=new Node(-q->c,q->e); r=r->next; q=q->next; }
            else { long long c=p->c-q->c; if(c){ r->next=new Node(c,p->e); r=r->next; } p=p->next; q=q->next; }
        }
        return R;
    }

    // --- 选做 (1)：在 x 处求值 ---
    double eval(double x) const {
        double s = 0.0;
        for (Node* p=head->next; p; p=p->next) {
            // 使用 pow，若 e 很大可换为快速幂或霍纳法
            s += static_cast<double>(p->c) * std::pow(x, p->e);
        }
        return s;
    }

    // --- 选做 (4)：代数形式 ---
    string toAlgebra() const {
        Node* p = head->next;
        if (!p) return "0";
        ostringstream ss;
        bool first = true;
        while (p){
            long long c = p->c;
            int e = p->e;
            if (c == 0) { p=p->next; continue; }

            // 符号
            if (first) {
                if (c < 0) ss << "-";
            } else {
                ss << (c >= 0 ? "+" : "-");
            }

            long long absc = llabs(c);

            // 主体：按 e 分类
            if (e == 0) {                 // 常数
                ss << absc;
            } else if (e == 1) {          // x
                if (absc != 1) ss << absc;
                ss << "x";
            } else {                      // x^e
                if (absc != 1) ss << absc;
                ss << "x^" << e;
            }

            first = false;
            p = p->next;
        }
        return ss.str();
    }

    void printPairs(ostream& os=cout) const {
        Node* p=head->next; if(!p){ os<<"0 0\n"; return; }
        bool first=true;
        while(p){ if(!first) os<<' '; os<<p->c<<' '<<p->e; first=false; p=p->next; }
        os<<'\n';
    }
    void printAlgebra(ostream& os=cout) const { os << toAlgebra() << '\n'; }

    void clear(){ Node* p=head->next; while(p){auto t=p->next; delete p; p=t;} head->next=nullptr; }

private:
    Node* head;
    void copyFrom(const Poly& other){ head->next=nullptr; Node* p=other.head->next; while(p){ insertTerm(p->c,p->e); p=p->next; } }
    void destroy(){ clear(); delete head; head=nullptr; }
};

// ------- 演示 -------
// 输入：
// nA  c1 e1 c2 e2 ... c_nA e_nA
// x_value
// 输出：三行
// 1) A 的“系数-指数”序列
// 2) A 的代数形式（选做4）
// 3) A 在 x 处的值（选做1）
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int nA;
    if (!(cin >> nA)) return 0;
    vector<pair<long long,int>> terms(nA);
    for (int i=0;i<nA;++i) cin >> terms[i].first >> terms[i].second;

    double x; cin >> x;

    Poly A; A.buildFromPairs(terms);

    A.printPairs();            // 序列输出（原基本要求 2）
    A.printAlgebra();          // 选做 (4)：代数形式
    cout.setf(ios::fixed);     // 选做 (1)：在 x 处求值
    cout<<setprecision(6)<<A.eval(x)<<"\n";

    return 0;
}
