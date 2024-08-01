#include<bits/stdc++.h>

using namespace std;

struct TOOLS {
    typedef long long lint;

    // {{value, amount}}
    //{1, 3, 5, 5, 7} -> {{1, 1}, {3, 1}, {5, 2}, {7, 1}}
    vector<pair<lint, lint>> rebute (vector<lint> vec) {
        vector<pair<lint, lint>> res;

        std::sort(vec.begin(), vec.end());

        vector<lint> temp(vec);
        temp.erase(unique(temp.begin(), temp.end()), temp.end());

        vector<int> cnt(vec.size() + 10, 0);

        lint prev = -4e18 ; //dummy value
        for (int i=0,idx =-1;i<vec.size();i++) {
            if (prev != vec[i]) idx++;

            prev = vec[i];
            cnt[idx]++;
        }

        for (int i=0;i<temp.size();i++) {
            res.emplace_back(temp[i], cnt[i]);
        }
        return res;
    }



    //diff [-INF, 1] ~ [2, INF] 에 대한 판정 0 : 잇지 않겠다, 1 : 잇는 것을 허용한다.
    vector<pair<lint, lint>> sweepLine (vector<pair<lint, lint>> vec, int diff) {
        typedef pair<lint, lint> pt;
        sort(vec.begin(), vec.end(), [&] (pt a, pt b) {
            if (a.first == b.first) return a.second < b.second;
            return a.first < b.first;
        });

        vector<pair<lint, lint>> sweeped;
        pair<lint, lint> res = vec[0];

        for (int i=1;i<vec.size();i++) {
            if ((vec[i].first <= res.second) ||
                        (vec[i].first - 1 == res.second && diff)) {
                res.second = max(res.second, vec[i].second);

                if (i + 1 == vec.size()) sweeped.push_back(res);
                        } else {
                            //sweeping 이 한줄로 이어지지 못함.
                            sweeped.push_back(res);
                            res = vec[i];
                        }
        }

        return sweeped;
    }
};
