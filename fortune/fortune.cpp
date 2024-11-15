#include<bits/stdc++.h>

typedef long long lint;
using namespace std;

int N;
const long double EPS = 1e-11, INF = 4e18;

bool isSame (long double a, long double b) {
    if (fabsl(a - b) <= EPS) return true;
    return false;
}

struct Point {
    long long x, y;
    int idx;

    Point () : x(0), y(0),idx(-1) {}
    Point (long long x, long long y, int idx = -1) : x(x), y(y), idx(idx) {}

    bool operator< (const Point& b) const {
        if (x == b.x) return y < b.y;
        return x < b.x;
    }
};

struct Vector2D {
    using vt = Vector2D;
    long double x, y;

    Vector2D () : x(0), y(0) {}
    Vector2D (double x, double y) : x(x), y(y) {}
    Vector2D (long double x, long double y) : x(x), y(y) {}
    Vector2D (Point p) : x(p.x), y(p.y) {}

    explicit Vector2D (pair<long double, long double> pr) : x(pr.first), y(pr.second) {}

    long double dotProd (const vt b) const {
        return x * b.x + y * b.y;
    }

    vt operator* (long double scalar) const {
        return {x * scalar, y * scalar};
    }

    vt operator- (const vt b) const {
        return {x - b.x, y - b.y};
    }

    vt operator+ (const vt b) const {
        return {x + b.x, y + b.y};
    }

    bool operator< (const vt b) const {
        if (isSame(x, b.x)) return y < b.y;
        return x < b.x;
    }

    long double cross(const vt o) const {
        return x * o.y - y * o.x;
    }

    vt rot () const {
        return {-y, x};
    }

    long double size() {
        return hypotl(x, y);
    }
};

long double sweepX;
int NOW_ID = 1;

struct Arc {
    using pt = Point;
    using vt = Vector2D;

    // C : 초점, next : 다음 Arc의 초점
    mutable pt c, next;
    mutable int idx, id;

    Arc (pt c, pt next, int idx) : c(c), next(next), idx(idx), id(++NOW_ID) {}

    long double getY (long double x) const {
        //다음 Arc가 없어서, 모든 지점커버.
        if (next.y == INF) return INF;

        ///e
        if (c.x == next.x) return (long double) (c.y + next.y) / 2.0;
        if (c.x == x) return c.y;

        vt med = (vt(c) + vt(next)) * 0.5;
        vt dir = (vt(c) - med).rot();
        long double D = (x - c.x) * (x - next.x);
        assert(D>=0);
        return med.y + ((med.x - x) * dir.x + sqrtl(D) * dir.size()) / dir.y;
    }

    bool operator<(const long double &y) const {
        if (isSame(getY(sweepX), y)) return false;
        return getY(sweepX) < y;
    }
    bool operator<(const Arc &o) const {
        long double L = getY(sweepX);
        long double R = o.getY(sweepX);

        if (!isSame(L, R)) return L < R;
        return c.x < o.c.x;
    }
};

template<size_t MAX>
struct fortune {
    long double deletedTime[MAX*5];
    using beach = multiset<Arc, less<>>;
    beach line;

    vector<Point> points;
    Point pts[MAX];
    vector<Vector2D> voronoiPoint[MAX];
    vector<pair<int, int>> edges;

    int N;

    struct vertexEvent {
        long double x;
        beach::iterator it;
        int id;

        bool operator< (const vertexEvent o) const {
            return x > o.x;
        }
    };

    fortune (vector<Point> vc) {
        N = vc.size();

        for (int i=0;i<N;i++) {
            pts[i] = Point(vc[i].x, vc[i].y, i);
        }
    }

    // intersection point of lines ab and cd. Precondition is that they aren't collinear

    Vector2D lineLine(Vector2D a, Vector2D b, Vector2D c, Vector2D d) {
        return a + (b - a) * ((c - a).cross(d - c) / (b - a).cross(d - c));
    }

    // circumcircle of points a, b, c. Precondition is that abc is a non-degenerate triangle.
    Vector2D circumCenter(Vector2D a, Vector2D b, Vector2D c) {
        b = (a + b) * 0.5;
        c = (a + c) * 0.5;
        return lineLine(b, b + (b - a).rot(), c, c + (c - a).rot());
    }

    priority_queue<vertexEvent , vector<vertexEvent>> vertexQueue;

    using vt = Vector2D;
    using pt = Point;

    long double crossProduct(long double x1, long double y1, long double x2, long double y2) {
        return x1 * y2 - y1 * x2;
    }

    // find p1-p2 and p3-p4 's cross Point
    Vector2D findCrossPoint(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4) {
        long double dx1 = p2.x - p1.x, dy1 = p2.y - p1.y;
        long double dx2 = p4.x - p3.x, dy2 = p4.y - p3.y;

        long double det = crossProduct(dx1, dy1, dx2, dy2);

        long double t1 = crossProduct(p3.x - p1.x, p3.y - p1.y, dx2, dy2) / det;
        long double t2 = crossProduct(p3.x - p1.x, p3.y - p1.y, dx1, dy1) / det;

        return Vector2D(p1.x + t1 * dx1, p1.y + t1 * dy1);
    }

    int ccw (Point a, Point b, Point c) {
        lint temp = (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
        if (temp > 0) return 1;
        if (temp < 0) return -1;
        return 0;
    }

    int ccw (Vector2D a, Vector2D b, Vector2D c) {
        long double res = (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
        if (isSame(res, 0)) return 0;
        if (res < 0) return -1;
        return 1;
    }

    bool checkCross (vt a, vt A, vt b, vt B) {
        bool result = false;

        result |= inside(a, A, b) || inside(a, A, B);
        result |= inside(b, B, a) || inside(b, B, A);

        int res1 = ccw(a, A, b) * ccw(a, A, B);
        int res2 = ccw(b, B, a) * ccw(b, B, A);

        result |= res1 < 0 && res2 < 0;

        return result;
    }

    bool inside (vt a, vt b, vt c) {
        int cw = ccw(a, b, c);

        long double mnX = min(a.x, b.x), mxX = max(a.x, b.x);
        long double mnY = min(a.y, b.y), mxY = max(a.y, b.y);

        return mnX -EPS <= c.x && c.x <= EPS + mxX && mnY - EPS <= c.y && c.y <= mxY + EPS && cw == 0;
    }

    void addEdge (int i, int j) {
        if (i < 0 || j < 0) return;
        edges.emplace_back(i, j);
    }

    void getRemoveTime (beach::iterator it) {
        if (it == line.begin() || it == prev(line.end())) return;

        auto prevArc = prev(it) ;

        if (ccw(prevArc->c, it->c, it->next) > 0) {
            vt c = circumCenter(vt(it->c), vt(it->next), vt(prevArc->c));
            long double x = c.x + (c - vt(it->c)).size();

            deletedTime[it->id] = x;
            vertexQueue.push({x, it, it->id});
        } else {
            deletedTime[it->id] = INF;
        }
    }

    void insertVoronoiPoint (Vector2D p, const vector<int>& indexes) {
        for (int e : indexes) {
            if (e < 0 || e >= N) continue;
            voronoiPoint[e].push_back(p);
        }
    }

    void vertexEvent (lint until) {
        while (vertexQueue.size() && vertexQueue.top().x <= until) {
            auto [x, it, id] = vertexQueue.top(); vertexQueue.pop();

            // 삭제시간이 업데이트 되어서, 이건 더미 값이다.
            if (deletedTime[id] != x) continue;
            sweepX = x;

            auto PREV = prev(it);
            auto NEXT = next(it);

            vector<int> receiver = {it->idx, PREV->idx, NEXT->idx};

            auto res = circumCenter(vt(it->c), vt(it->next), vt(PREV->c));
            insertVoronoiPoint(res, receiver);
            addEdge(PREV->idx, NEXT->idx);

            line.erase(it);

            PREV->next = NEXT->c;

            getRemoveTime(PREV);
            getRemoveTime(NEXT);

            deletedTime[id] = INF;
        }
    }

    void pointEvent (Point p) {
        //분리해야하는 호 c이다.
        auto c = line.lower_bound(p.y);

        addEdge(c->idx, p.idx);

        if (isSame(p.y, c->getY(p.x))) {
            auto b = line.insert(c, Arc(p, c->next, p.idx));
            auto a = line.insert(b, Arc(c->c, p, c->idx));

            vector<int> receiver = {p.idx, c->idx, c->next.idx};
            auto res = circumCenter(vt(c->c), vt(c->next), vt(p));
            insertVoronoiPoint(res, receiver);
            addEdge(p.idx, c->next.idx);

            deletedTime[c->id] = INF;
            line.erase(c);

            getRemoveTime(next(b));
            getRemoveTime(b);
            getRemoveTime(a);
        } else {
            auto b = line.insert(c, Arc(p, c->c, p.idx));
            auto a = line.insert(b, Arc(c->c, p, c->idx));

            getRemoveTime(c);
            getRemoveTime(b);
            getRemoveTime(a);
        }
    }

    void solve(lint E = 1e6) {
        assert(N >= 3);

        for (int i=0;i<N;i++) {
            points.push_back(pts[i]);
        }

        points.emplace_back(0, E);
        points.emplace_back(0, -E);
        points.emplace_back(E, 0);

        std::sort(points.begin(), points.end());

        line.insert(Arc(pt(-E, 0), pt(INF, INF), -1));

        for (Point now : points) {
            vertexEvent(now.x);
            sweepX = now.x;
            pointEvent(now);
        }

        vertexEvent(8e18);

        for (int i=0;i<N;i++) {
            assert(voronoiPoint[i].size() >= 3);

            std::sort(voronoiPoint[i].begin(), voronoiPoint[i].end());
            const vt standard = voronoiPoint[i][0];

            std::sort(voronoiPoint[i].begin()+1, voronoiPoint[i].end(), [&] (vt a, vt b) {
                return ccw(standard, a, b) < 0;
            });
        }
    }
};
