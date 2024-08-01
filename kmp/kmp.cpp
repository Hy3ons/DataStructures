#include<bits/stdc++.h>
using namespace std;

vector<int> kmp (string &a) {
    vector<int> result(a.size(), 0);

    for (int i=1, j = 0;i<a.size();i++) {
        while(j > 0 && a[i] != a[j]) {
            j = result[j-1];
        }

        if (a[i] == a[j]) result[i] = ++j;
    }

    return result;
}

int matching (string a, string b) {
    // counting b in a
    
    vector<int> fail = kmp(b);
    int count = 0;

    for (int i=0,j = 0;i<a.size();i++) {
        while(j > 0 && a[i] != b[j]) j = fail[j-1];

        if (a[i] == b[j]) {
            if (++j == b.size()) {
                //matching occured 
                // a[i-j+1 : i] == b[0 :j-1] 0-based

                j = fail[j-1];
                count++;
            }
        }
    }
    
    return count;
}