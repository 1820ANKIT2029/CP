#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

const double EPS = 1e-9;
const double PI = acos(-1.0);

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}

    Point operator + (const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator - (const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    Point operator * (double k) const {
        return Point(x * k, y * k);
    }

    Point operator / (double k) const {
        return Point(x / k, y / k);
    }

    bool operator == (const Point& other) const {
        return abs(x - other.x) < EPS &&
               abs(y - other.y) < EPS;
    }

    bool operator < (const Point& other) const {
        if(abs(x - other.x) > EPS)
            return x < other.x;
        return y < other.y;
    }
};

double dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

double norm(const Point& a) {
    return dot(a, a);
}

double angle(const Point& a, const Point& b) {
    return acos(dot(a, b) / sqrt(norm(a) * norm(b)));
}

Point rotate(const Point& p, double theta) {
    return Point(
        p.x * cos(theta) - p.y * sin(theta),
        p.x * sin(theta) + p.y * cos(theta)
    );
}

/*
    >0 counter-clockwise, <0 clockwise, =0 collinear
*/
int orientation(const Point& a, const Point& b, const Point& c) {
    double val = cross(b - a, c - a);

    if(abs(val) < EPS) return 0;
    return (val > 0 ? 1 : -1);
}

// checks if point q lies on segment pr (assuming they are collinear)
bool onSegment(const Point& p, const Point& q, const Point& r) {
    return orientation(p, q, r) == 0 &&
           q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
           q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
}

// checks if segment p1q1 and segment p2q2 intersect
bool doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case: Straddling
    if (o1 != o2 && o3 != o4) 
        return true;

    // Special cases: Collinear and overlapping
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

// formula: 2 * area = |cross(x1, x2) + cross(x2, x3) + ... + cross(xn, x1)|
long long polygonArea2(const vector<Point>& poly) {
    long long area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n; 
        area += cross(poly[i], poly[j]);
    }
    return abs(area);
}

