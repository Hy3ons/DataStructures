#include<bits/stdc++.h>
using namespace std;

template <size_t MAX>
struct checkBPgraph {
    int my[MAX+3];
    queue<int> qu;

    bool isBPgraph (vector<int> (&load)[], int N) {
        fill(begin(my), end(my), -1);

        for (int i=1;i<=N;i++) {
            if (my[i] != -1) continue;
            qu.push(i); my[i] = 0;

            while(qu.size()) {
                int now = qu.front(); qu.pop();

                for (int next : load[now]) {
                    if (my[next] == -1) {
                        my[next] = my[now] ^ 1;
                        qu.push(next);
                    } else {
                        if (my[next] == my[now]) return false;
                    }
                }
            }
        }

        return true;
    }
};