#include<bits/stdc++.h>
using namespace std;

class SEG {
    typedef long long lint;
    const lint INF = -1e17;

public:
    int N;
    class node{
    public:
        lint l, r, sum, mx;

        int dummy;

        node() {
            dummy = 1;
        }

        node(lint value) {
            l = r = sum = mx = value;
        }
    };

    vector<node> tree;

    SEG (vector<int> &vc) {
        N = vc.size();

        tree.resize(N*4);
        init(vc, 1, 1, N);
    }

private:
    node sumNode (node a, node b) {
        if (a.dummy) return b;
        if (b.dummy) return a;

        node res;
        res.l = max(a.l, a.sum + b.l);
        res.r = max(b.r, a.r + b.sum);
        res.mx = max({a.mx, b.mx, a.r + a.l});
        res.sum = a.sum + b.sum;

        res.dummy = 0;

        return res;
    }

    void init (vector<int> &vc, int n, int s, int e) {
        if (s == e) {
            tree[n] = node(vc[s-1]);
            return;
        }

        int mid = s + e >> 1;
        init(vc, n << 1, s, mid);
        init(vc, n << 1 | 1, mid+1, e);

        tree[n] = sumNode(tree[n << 1], tree[n << 1 | 1]);
    }

    node query (int n, int s, int e, int l, int r) {
        if (r < s || e < l) return node();

        if (s <= l && r <= e)  {
            return tree[n];
        }

        int mid = l + r >> 1;

        node ll = query(n<<1, s, e, l, mid);
        node rr = query(n<<1|1, s, e, mid+1, r);

        return sumNode(ll, rr);
    }

    void update (int n, int l, int r, int idx, lint value) {
        if (l == r) {
            tree[n] = node(value);
            return;
        }

        int mid = l + r >> 1;

        if (idx <= mid) update(n << 1 , l, mid, idx, value);
        else update(n << 1 | 1, mid+1, r, idx, value);

        tree[n] = sumNode(tree[n<<1], tree[n << 1 | 1]);
    }
public:
    node query (int l, int r) {
        return query(1, l, r, 1, N);
    }

    void update (int idx, lint value) {
        update(1, 1, N, idx, value);
    }
};