
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