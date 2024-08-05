```c++
//모든 함수의 매개변수 구간 [l, r] 은 1-based로 집어넣으면 됩니다.

SPLAY spl;
//특정 노드를 없앱니다.
void destroy (Node *a)
void erase (Node *a)
void erase (lint key)

//key 값과 value 값을 넣습니다. 있다면 value를 덮어씁니다.
void push (lint key, lint value)
//노드 a 바로 왼쪽에 노드 b가 오게 push합니다.
void pushLeft (Node *a, Node* b)
//노드 a오른쪽에 바로 노드 b가 오게 push합니다.
void pushRight (Node *a, Node *b)


//변수 order(1-based) 번째의 노드를 SPLAY 합니다. 구간 범위 아웃일 경우 False
bool findKth (int order)
//key 값을 가진 노드를 splay 시킵니다. key값을 가진 노드가 없으면 False를 리턴합니다.
bool find (lint key)
//노드의 순서를 알아냅니다. 1-based로 리턴합니다.
int getOrder(Node* n)

//구간 [l, r]을 분리 해 냅니다. 리턴된 노드는 루트가 아님에 유의 하십시오.
//구간이 공집합일 경우, 크기가 0이하일 경우 nullptr을 리턴합니다.
Node* segment (int l, int r)

//Dp 값과 똑같은 value 를 가진 노드를 splay 합니다.
//노드 a전체 구간에 대해 탐색을 시도합니다.
//구간에 대해 검색하고 싶을 땐, findDp(segment(l, r)) 으로 응용하십시오.
Node* findDp (Node *a)

bool empty ()
int size ()
void clear()
void printALL (Node *a)

//etc
//reverse 함수를 호출할 경우 key에 따라 넣는 정렬하는 splay 트리의 정렬이 무너집니다.
//splay tree를 이해하고 있는 사람이 아니라면 사용하지 마십시오.
//사용한다면, 이후에 key값을 사용하는 push(), find(), erase(key)함수를 사용하지 마십시오.
//웬만하면 splay(), segment()를 사용한 원시접근을 권장합니다.
//구간 내 원소의 위치를 뒤집습니다.
bool reverse(int l, int r) 
```
