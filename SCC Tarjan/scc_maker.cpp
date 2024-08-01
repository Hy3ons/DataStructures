#include<bits/stdc++.h>

using namespace std;

template <size_t MAX>
struct sccMaker{
    vector<int> seed, load[MAX], re[MAX], stack;
    int mySCC[MAX], visit[MAX];

    void addEdge (int from, int go) {
        load[from].push_back(go);
        re[go].push_back(from);

        seed.push_back(from);
        seed.push_back(go);
    }

    void clear () {
        for (int & i : seed) {
            mySCC[i] = visit[i] = 0;
            load[i].clear(); re[i].clear();
        }

        stack.clear();
        seed.clear();
    }

    void function () {
        for (int & i : seed) {
            if (!visit[i]) dfs(i);
        }

        reverse(begin(stack), end(stack));
        int cnt = 1;

        for (int & i : stack) {
            if (!mySCC[i]) dfs2(i, -1, cnt++);
        }
    }

    void dfs (int node, int prev = -1) {
        visit[node] = 1;

        for (int & i : load[node]) if (i != prev) {
            if (!visit[i]) dfs(i, node);

        }

        stack.push_back(node);
    }

    void dfs2 (int node, int prev, int mySccNum) {
        mySCC[node] = mySccNum;

        for (int & i : re[node]) if (i != prev){
            if (!mySCC[i]) dfs2(i, node, mySccNum);
        }
    }

    bool isSame (int n1, int n2) {
        if (!mySCC[n1] || !mySCC[n2]) return false;
        return mySCC[n1] == mySCC[n2];
    }
};