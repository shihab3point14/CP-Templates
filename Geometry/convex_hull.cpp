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


/**
 * Convex Hull - Monotone Chain (Andrew's Algorithm)
 * Time Complexity: O(N log N)
 */
struct MonotoneChain {
  struct pt {
    double x, y;
    bool operator<(const pt &other) const {
      return x < other.x || (x == other.x && y < other.y);
    }
  };

  static double cross_product(pt a, pt b, pt c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
  }

  static vector<pt> get_hull(vector<pt> pts, bool include_collinear = false) {
    int n = pts.size();
    if (n <= 2)
      return pts;

    sort(pts.begin(), pts.end());
    vector<pt> hull;

    // Build Upper Hull
    for (int i = 0; i < n; i++) {
      while (hull.size() >= 2) {
        double cp = cross_product(hull[hull.size() - 2], hull.back(), pts[i]);
        if (include_collinear ? cp >= 0 : cp > 0)
          break;
        hull.pop_back();
      }
      hull.push_back(pts[i]);
    }

    // Build Lower Hull
    int upper_size = hull.size();
    for (int i = n - 2; i >= 0; i--) {
      while (hull.size() > upper_size) {
        double cp = cross_product(hull[hull.size() - 2], hull.back(), pts[i]);
        if (include_collinear ? cp >= 0 : cp > 0)
          break;
        hull.pop_back();
      }
      hull.push_back(pts[i]);
    }

    hull.pop_back(); // Remove redundant last point
    return hull;
  }

  static double area(const vector<pt> &hull) {
    double a = 0;
    for (int i = 0; i < hull.size(); i++) {
      int j = (i + 1) % hull.size();
      a += hull[i].x * hull[j].y - hull[j].x * hull[i].y;
    }
    return abs(a) / 2.0;
  }

  // NEW: Added Perimeter function
  static double perimeter(const vector<pt> &hull) {
    double p = 0;
    for (int i = 0; i < hull.size(); i++) {
      int j = (i + 1) % hull.size();
      double dx = hull[i].x - hull[j].x;
      double dy = hull[i].y - hull[j].y;
      p += sqrt(dx * dx + dy * dy);
    }
    return p;
  }
};

/*
  vector<MonotoneChain::pt> points(n);
  for (int i = 0; i < n; i++) {
    cin >> points[i].x >> points[i].y;
  }

  // Process Convex Hull
  vector<MonotoneChain::pt> hull = MonotoneChain::get_hull(points, false);

  // Results
  cout << "Points in Hull: " << hull.size() << "\n";
  cout << fixed << setprecision(2);
  cout << "Area: " << MonotoneChain::area(hull) << "\n";
  cout << "Perimeter: " << MonotoneChain::perimeter(hull) << "\n";
*/