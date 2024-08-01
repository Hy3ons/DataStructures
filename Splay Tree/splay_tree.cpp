#include<bits/stdc++.h>

using namespace std;

struct SPLAY {
    typedef long long lint;
    struct Node {
        Node *p = nullptr, *l = nullptr, * r = nullptr;
        lint key, value, size = 1, dp;

        lint lvalue = 0;
        int reLazy = 0, lazy = 0;

        Node (lint key, lint value = 0) : key(key), value(value) {
            update();
        }

        void update () {
            size = 1;
            dp = value;

            if (l) {
                l->propagate();
                size += l->size;
                dp = min(dp, l->dp);
            }

            if (r) {
                r->propagate();
                size += r->size;
                dp = min(dp, r->dp);
            }
        }

        void propagate () {
            if (reLazy) {
                swap(l, r);
                if (l) {
                    l->reLazy ^= 1;
                }

                if(r) {
                    r->reLazy ^= 1;
                }
                reLazy = 0;
            }

            if (!lazy) return;

            key += lvalue;

            if (l) {
                l->lazy = 1;
                l->lvalue += lvalue;
            }

            if (r) {
                r->lazy = 1;
                r->lvalue += lvalue;
            }

            lazy = lvalue = 0;
        }
    };

    map<lint, Node*> nodes;
    Node *root;

    void rotate (Node* a) {
        Node* parent = a->p, *leaf;

        parent->propagate(); a->propagate();

        if (parent->l == a) {
            leaf = parent->l = a->r;
            a->r = parent;
        } else {
            leaf = parent->r = a->l;
            a->l = parent;
        }

        a->p = parent->p;
        parent->p = a;

        if (leaf) {
            leaf->p = parent;
        }

        if(a->p) {
            if (a->p->l == parent) {
                a->p->l = a;
            } else {
                a->p->r = a;
            }
        } else {
            root = a;
        }

        parent->update();
        a->update();
    }

    //노드를 스플레이 시킵니다. 기본적으로 전파와 업데이트를 한번 수행합니다.
    void splay (Node* a) {
        a->propagate();
        a->update();

        while(a->p) {
            if (a->p->p)
                rotate((a->p->p->l == a->p) == (a->p->l == a) ? a->p : a);
            rotate(a);
        }
    }

    //노드의 순서를 알아냅니다. 1-based로 리턴합니다.
    int getOrder (Node *a) {
        splay(a);
        return a->l ? a->l->size + 1 : 1;
    }

    //특정 노드를 없앱니다.
    void erase (Node *a) {
        destroy(a);
    }

    void erase (lint key) {
        if (find(key)) destroy(root);
    }

    //특정 노드를 없앱니다.
    void destroy (Node *a) {
        splay(a);

        //자식이 없다.
        if (!a->l && !a->r) {
            root = 0;

            delete a;
            return;
        }

        if (!a->l) {
            root = a->r;
            root->p = 0;

            delete a;
            return;
        }

        if (!a->r) {
            root = a->l;
            root->p = 0;

            delete a;
            return;
        }

        Node *left = a->l;
        Node *right = a->r;

        left->p = right->p = 0;

        left->propagate(); right->propagate();

        while(left->r) {
            left = left->r;
            left->propagate();
        }

        splay(left);

        left->r = right;
        right->p = left;

        left->update();

        root = left;
        delete a;
    }

    //key 값과 value 값을 넣습니다. 있다면 value를 덮어씁니다.
    void push (lint key, lint value) {
        if (!root) {
            Node* a= new Node(key, value);
            root = a;
            return;
        }

        Node* now = root;

        while(1) {
            now->propagate();
            if (key < now->key) {
                if (now->l) now = now->l;
                else {
                    Node* a= new Node(key, value);

                    now->l = a; a->p = now;

                    splay(a);
                    return;
                }
            } else if (key > now->key) {
                if (now->r) now = now->r;
                else {
                    Node* a= new Node(key, value);

                    now->r = a;a->p = now;

                    splay(a);
                    return;
                }
            } else {
                splay(now);
                now->value = value;

                now->update();
                return;
            }
        }
    }

    //변수 order(1-based) 번째의 노드를 SPLAY 합니다. 구간 범위 아웃일 경우 False
    bool findKth (int order) {
        if (size() < order) return false;

        Node* now = root;
        splay(root);

        while(1) {
            now->propagate();
            if (now->l == nullptr && now->r == nullptr) break;

            if (now->l && now->r) {
                if (now->l->size + 1 == order) break;

                if (now->l->size < order) {
                    order -= now->l->size + 1;
                    now = now->r;
                } else {
                    now = now->l;
                }
            } else if (now->l) {
                if (now->l->size + 1 == order) break;

                now = now->l;
            } else {
                if (order == 1) break;

                now = now->r;
                order--;
            }
        }

        splay(now);
        return true;
    }

    //key 값을 가진 노드를 splay 시킵니다. key값을 가진 노드가 없으면 False를 리턴합니다.
    bool find (lint key) {
        Node* now = root;
        splay(now);

        while(1) {
            now->propagate();
            if (key < now->key) {
                if (now->l) now = now->l;
                else break;
            } else if (key > now->key) {
                if (now->r) now = now->r;
                else break;
            } else break;
        }

        splay(now);
        return now->key == key;
    }

    bool reverse (int l, int r) {
        if (r < l) return false;
        segment(l, r)->reLazy ^= 1;
        return true;
    }

    //구간 [l, r]을 분리 해 냅니다. 리턴된 노드는 루트가 아님에 유의 하십시오.
    Node* segment (int l, int r) {
        if (r < l) return nullptr;

        r = min(r, size());
        l = max(l, 1);

        if (l == 1 && r == size()) {
            root->propagate();
            return root;
        } else if (l == 1) {
            findKth(r+1);
            root->l->propagate();

            return root->l;
        } else if (r == size()) {
            findKth(l-1);
            root->r->propagate();

            return root->r;
        } else {
            findKth(r+1);

            Node *rroot = root;
            root = rroot->l;

            rroot->l = 0;
            root->p = 0;

            findKth(l-1);

            Node *res = root->r; res->propagate();

            root->p = rroot; rroot->l = root;

            rroot->update();

            root = rroot;
            return res;
        }
    }

    //Splay Tree 안의 노드의 유무를 리턴합니다.
    bool empty () {
        if (!root) return true;
        return false;
    }

    //Splay Tree 안의 노드의 갯수를 리턴합니다.
    int size () {
        if (root) return root->size;
        return 0;
    }

    void printALL (Node *a) {
        a->propagate();
        if (a->l) printALL(a->l);
        cout << a->key << ' ';
        if (a->r) printALL(a->r);
    }

    void clearing (Node *a) {
        if (a->l) clearing (a->l);
        if (a->r) clearing (a->r);

        delete a;
    }

    Node* findDp (Node *a) {
        if (!a) return a;

        while(1) {
            a->propagate();
            if (a->l) a->l->propagate();
            if (a->r) a->r->propagate();

            if (a->dp == a->value) {
                splay(a);
                return a;
            }

            if (a->l && a->l->dp == a->dp) {
                a = a->l;
            } else {
                a = a->r;
            }
        }
    }

    void clear () {
        if (!root) return;
        clearing(root); root = 0;
    }

};
