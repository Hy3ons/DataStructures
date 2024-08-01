#include<bits/stdc++.h>
using namespace std;

// Persistance Segment Tree : 0부터 N까지의 수를 관리합니다.
class PSEG {
public:
	size_t N;

    class node {
    public:
        int sum = 0;
        node *l, *r;

        node() : sum(0) {}
    };

    vector<node*> nodes;

    PSEG (int N) : N(N) {
        nodes.push_back(init(0, N));
    }

    PSEG (vector<int> &vec, int N) : N(N) {
        nodes.push_back(init(0, N));

        for (int e : vec) {
            nodes.push_back(update(nodes.back(), 1, N, e, 1));
        }
    }

private:
    node* init (int l, int r) {
        if (l == r) {
            return new node();
        }

        node *now = new node();

        int mid = l + r >> 1;
        now->l = init(l, mid);
        now->r = init(mid+1, r);

        return now;
    }

    int query (node* up, node* down, int s, int e, int l, int r) {
        if (r < s || e < l) return 0;

        if (s <= l && r <= e) {
            return up->sum - down->sum;
        }

        int mid = l + r >> 1;

        return query(up->l, down->l, s, e, l, mid)
            + query(up->r, down->r, s, e, mid+1, r);
    }
public :
    //l, r 인수는 1-based 입니다.
    int query (int l, int r, int mn, int mx) {
        return query(nodes[r], nodes[l-1], max(0, mn), min((int) N, mx), 0, N);
    }

    //num 인수를 받아, 해당 수를 마지막 분기에서 업데이트 시킨 것을 새롭게 마지막에 넣습니다.
    void push (int num) {
        nodes.push_back(update(nodes.back(), 0, N, num, 1));
    }

    //몇개의 수가 저장되고 있는지 리턴합니다.
    size_t size () {
        return nodes.size() - 1;
    }

    // now로 기록된 매개체를 통해 업데이트된 새로운 트리의 구조체를 리턴합니다.
    node* update (node *now, int l, int r, int idx, int d) {
        if (r < idx  || idx < l) return now;

        if (l == r && l == idx) {
            node *temp = new node();
            temp->sum = now->sum + d;
            return temp;
        }

        int mid = l + r >> 1;

        node *temp = new node();
        temp->l = update(now->l, l, mid, idx, d);
        temp->r = update(now->r, mid+1, r, idx, d);

        temp->sum = temp->l->sum + temp->r->sum;

        return temp;
    }
};