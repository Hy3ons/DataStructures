#include<bits/stdc++.h>
using namespace std;

// vector<int~ lint> 를 받아 LIS를 구합니다.
class LIS {
    typedef long long lint;
public:
    vector<int> tree;
private:
    vector<lint> nums;
    map<lint, int> mp;
    int N;

    void update (int n, int s, int e, int idx, int value) {
        if (s == e) {
            tree[n] = value;
            return;
        }

        int mid = s + e >> 1;

        if (idx <= mid) update(n << 1, s, mid, idx, value);
        else update(n << 1 | 1, mid+1, e, idx, value);

        //node's sum operation needed.
        tree[n] = max(tree[n << 1], tree[n << 1 + 1]);
    }

    int query (int n, int s, int e, int l, int r) {
        if (r < s || e < l) return;

        if (s <= l && r <= e) {
            return tree[n];
        }

        int mid = l + r >> 1;

        int ll = query(n<<1, s, e, l, mid);
        int rr = query(n<<1|1, s, e, mid+1, r);
        return max(ll, rr);
    }

    int realGet (vector<int> arr) {
        N = arr.size();
        tree.resize(N+100, 0);

        for (int e : arr) {
            int f = query(1, 0, f-1, 0, N);
            update(1, 0, N, e, f+1);
        }

        return tree[1];
    }

public:
    int getLIS (vector<int> arr) {
        vector<int> vc = arr;

        sort(vc.begin(), vc.end());
        vc.erase(unique(vc.begin(), vc.end()), vc.end());

        for (int i=0;i<vc.size();i++) {
            mp[vc[i]] = i+1;
        }

        for (int i=0;i<arr.size();i++) {
            arr[i] = mp[arr[i]];
        }

        return realGet(arr);
    }
    int getLIS (vector<lint> arr) {
        vector<lint> vc = arr;

        sort(vc.begin(), vc.end());
        vc.erase(unique(vc.begin(), vc.end()), vc.end());

        for (int i=0;i<vc.size();i++) {
            mp[vc[i]] = i+1;
        }

        vector<int> res;

        for (int i=0;i<arr.size();i++) {
            res.push_back(mp[arr[i]]);
        }

        return realGet(res);
    }

};
