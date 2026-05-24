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

bool inPolygon(const std::vector<Point>& poly, Point p) {
    int n = poly.size();
    int count = 0;
    
    for(int i = 0; i < n; i++) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        if (orientation(a, b, p) == 0 && onSegment(a, p, b)) return true;
    
        if (a.y > b.y) std::swap(a, b);
        if (p.y > a.y && p.y <= b.y)
            if (orientation(a, b, p) == 1) count++;
    }
    
    return (count % 2 == 1);
}

/*
    Convex Hull: Given a set of points, find the smallest convex polygon that can enclose all the points.
    Time Complexity: O(n log n)

    algos are: 
    - graham's scan: sorts points by polar angle and constructs hull using a stack. O(n log n)
    - andrew's monotone chain: sorts points by x-coordinate and constructs lower and upper hulls separately. O(n log n)
*/

// andrew's monotone chain algorithm for convex hull
vector<Point> convex_hull(vector<Point>& pts) {
    int n = pts.size(), k = 0;
    if (n <= 2) return pts;
    
    vector<Point> hull(2 * n);
    sort(pts.begin(), pts.end());
    
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && orientation(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }

    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && orientation(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }
    
    hull.resize(k - 1);
    return hull;
}

long long distSq(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Rotating Calipers: Given a convex polygon, find the pair of points that are farthest apart (the diameter of the polygon).
long long polygonDiameter(const vector<Point>& hull) {
    int n = hull.size();
    if (n <= 1) return 0;
    if (n == 2) return distSq(hull[0], hull[1]);
    
    int j = 1;
    long long maxDistSq = 0;
    
    for (int i = 0; i < n; i++) {
        Point currentEdgeStart = hull[i];
        Point currentEdgeEnd = hull[(i + 1) % n];
        
        while (true) {
            Point next_j = hull[(j + 1) % n];
            
            long long area1 = abs(cross(currentEdgeEnd - currentEdgeStart, hull[j] - currentEdgeStart));
            long long area2 = abs(cross(currentEdgeEnd - currentEdgeStart, next_j - currentEdgeStart));
            
            if (area2 > area1) j = (j + 1) % n;
            else break;
        }
        
        maxDistSq = max({
            maxDistSq, 
            distSq(currentEdgeStart, hull[j]), 
            distSq(currentEdgeEnd, hull[j])
        });
    }
    
    return maxDistSq;
}

/*
    The Sweep-Line Algorithm example  (very important)
    Closest pair of points	O(n log n)
    Segment intersection	O(n log n)
    Rectangle union area	O(n log n)
    Skyline problem	        O(n log n)
    Interval overlap	    O(n log n)
*/

struct Point {
    long long x, y;
    int id;
};

bool compareX(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

struct compareY {
    bool operator()(const Point& a, const Point& b) const {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    }
};

long long distSq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Closest pair of points using sweep-line algorithm
long long closestPair(vector<Point>& pts) {
    int n = pts.size();
    if (n <= 1) return 0;

    sort(pts.begin(), pts.end(), compareX);
    set<Point, compareY> active_set;
    
    long long min_dist_sq = 4e18;
    double d = 2e9; 
    
    int left = 0;
    
    for (int i = 0; i < n; i++) {
        while (left < i && pts[i].x - pts[left].x >= d) {
            active_set.erase(pts[left]);
            left++;
        }
    
        Point lower_bound_pt = {pts[i].x, pts[i].y - (long long)ceil(d)};
        Point upper_bound_pt = {pts[i].x, pts[i].y + (long long)ceil(d)};
        
        auto start_it = active_set.lower_bound(lower_bound_pt);
        auto end_it = active_set.upper_bound(upper_bound_pt);
        
        for (auto it = start_it; it != end_it; ++it) {
            long long current_dist_sq = distSq(pts[i], *it);
            if (current_dist_sq < min_dist_sq) {
                min_dist_sq = current_dist_sq;
                d = std::sqrt(min_dist_sq);
            }
        }

        active_set.insert(pts[i]);
    }
    
    return min_dist_sq;
}

// Skyline problem
vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
    int n = buildings.size();

    vector<pair<int, int>> mp(2*n);
    int ind = 0;
    for(vector<int> &v: buildings) {
        mp[ind] = {v[0], v[2]};
        mp[ind+1] =  {v[1], -v[2]};
        ind += 2;
    }

    sort(mp.begin(), mp.end());

    multiset<int> pq;
    vector<vector<int>> ans;
    pq.insert(0);
    int i = 0, p, max_ = -1, h; n *= 2;
    while(i < n) {
        p = mp[i].first;
        while(i < n && mp[i].first == p) {
            if(mp[i].second > 0) pq.insert(mp[i].second);
            else pq.erase(pq.find(-mp[i].second));
            i++;
        }
        h = *pq.rbegin();
        if(max_ != h){ ans.push_back({p, h}); max_ = h;}
    }

    return ans;
}
