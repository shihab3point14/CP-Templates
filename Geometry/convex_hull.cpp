///////////////////////////////////////////////////////

// Graham's Scan algorithm
// Y-axis lower most point
// O(n)

struct pt {
    double x, y;
    bool operator == (pt const& t) const {
        return x == t.x && y == t.y;
    }
};

int orientation(pt a, pt b, pt c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // clockwise
    if (v > 0) return +1; // counter-clockwise
    return 0;
}

bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}
bool collinear(pt a, pt b, pt c) { return orientation(a, b, c) == 0; }

vector<pt> convex_hull(const vector<pt>& a, bool include_collinear = false) {
    vector<pt> b = a;
    pt p0 = *min_element(b.begin(), b.end(), [](pt x, pt y) {
        return make_pair(x.y, x.x) < make_pair(y.y, y.x);
    });
    sort(b.begin(), b.end(), [&](const pt& x, const pt& y) {
        int o = orientation(p0, x, y);
        if (o == 0)
            return (p0.x - x.x)*(p0.x - x.x) + (p0.y - x.y)*(p0.y - x.y)
                 < (p0.x - y.x)*(p0.x - y.x) + (p0.y - y.y)*(p0.y - y.y);
        return o < 0;
    });
    if (include_collinear) {
        int i = (int)b.size() - 1;
        while (i >= 0 && collinear(p0, b[i], b.back())) i--;
        reverse(b.begin() + i + 1, b.end());
    }
    vector<pt> st;
    for (int i = 0; i < (int)b.size(); i++) {
        while (st.size() > 1 && !cw(st[st.size()-2], st.back(), b[i], include_collinear))
            st.pop_back();
        st.push_back(b[i]);
    }
    if (!include_collinear && st.size() == 2 && st[0] == st[1])
        st.pop_back();
    return st;
}


////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////

/*

// Monotone chain algorithm
// first check left most and right most A and B, 
// In the event multiple such points exist, the lowest among the left (lowest Y-coordinate) is taken as A,
// and the highest among the right (highest Y-coordinate) is taken as B.
struct pt {
    double x, y;
};

int orientation(pt a, pt b, pt c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // clockwise
    if (v > 0) return +1; // counter-clockwise
    return 0;
}

bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}
bool ccw(pt a, pt b, pt c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o > 0 || (include_collinear && o == 0);
}

vector<pt> convex_hull(const vector<pt>& a, bool include_collinear = false) {
    int n = a.size();
    if (n < 2) return a;
    vector<pt> b = a;
    sort(b.begin(), b.end(), [](const pt& u, const pt& v) {
        return make_pair(u.x, u.y) < make_pair(v.x, v.y);
    });
    pt p1 = b.front(), p2 = b.back();
    vector<pt> up{p1}, down{p1};
    for (int i = 1; i < n; i++) {
        if (i == n-1 || cw(p1, b[i], p2, include_collinear)) {
            while (up.size() >= 2 && !cw(up[up.size()-2], up.back(), b[i], include_collinear))
                up.pop_back();
            up.push_back(b[i]);
        }
        if (i == n-1 || ccw(p1, b[i], p2, include_collinear)) {
            while (down.size() >= 2 && !ccw(down[down.size()-2], down.back(), b[i], include_collinear))
                down.pop_back();
            down.push_back(b[i]);
        }
    }
    vector<pt> res = up;
    for (int i = (int)down.size()-2; i > 0; i--)
        res.push_back(down[i]);
    return res;
}


/////////////////////////////////////////////////////////////////////////////////
// comment
// input vector<pt> arr;
// pt ---> pt.x and pt.y

*/