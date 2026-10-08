
 #include <bits/stdc++.h>
 using namespace std;

 /* GEOMETRY TEMPLATE FOR COMPETITIVE PROGRAMMING
 * ---------------------------------------------
 * 1. Basics & Constants
 * 2. Point Structure (Vector operations)
 * 3. Core Vector Functions (Dot, Cross, etc.)
 * 4. Line / Segment / Ray Functions (Distances, Intersections)
 * 5. Polygon Functions (Area, Convex Hull, Point-in-Poly)
 */

// =========================================================
// 1. Basic Setup & Constants
// =========================================================
const double EPS = 1e-9;
const double PI = acos(-1.0);
const double INF = 1e18; // Large value for comparisons

// Function to safely compare doubles
// Returns -1 if a < b, 0 if a == b, 1 if a > b
int dcmp(double a, double b) {
  if (abs(a - b) < EPS)
    return 0;
  return (a < b) ? -1 : 1;
}

// Convert degrees to radians
double toRad(double deg) { return deg * PI / 180.0; }
// Convert radians to degrees
double toDeg(double rad) { return rad * 180.0 / PI; }

// =========================================================
// 2. Point Structure
// =========================================================
struct Point {
  double x, y;

  // Constructors
  Point(double x = 0, double y = 0) : x(x), y(y) {}

  // Operator Overloading
  Point operator+(const Point &other) const {
    return {x + other.x, y + other.y};
  }
  Point operator-(const Point &other) const {
    return {x - other.x, y - other.y};
  }
  Point operator*(double s) const { return {x * s, y * s}; }
  Point operator/(double s) const { return {x / s, y / s}; }

  // Sorting: x first, then y
    bool operator<(const Point &other) const {
        if (dcmp(x, other.x) != 0) return dcmp(x, other.x) < 0;
        return dcmp(y, other.y) < 0;
    }

  bool operator==(const Point &other) const {
    return dcmp(x, other.x) == 0 && dcmp(y, other.y) == 0;
  }

  // Input/Output for easy debugging
  friend istream &operator>>(istream &is, Point &p) { return is >> p.x >> p.y; }
  friend ostream &operator<<(ostream &os, const Point &p) {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};

// =========================================================
// 3. Core Vector Operations
// =========================================================

// Dot Product: a.x*b.x + a.y*b.y
// Usage: Angle calculation, Projection. dot(A, B) = |A||B|cos(theta)
double dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }

// Cross Product: a.x*b.y - a.y*b.x
// Usage: Area, Orientation (Left/Right turn). |Cross| = |A||B|sin(theta)
double cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }

// Magnitude squared (avoid sqrt for comparisons)
double magSq(Point a) { return dot(a, a); }

// Magnitude (Length)
double len(Point a) { return sqrt(dot(a, a)); }

// Euclidean Distance between two points
double dist(Point a, Point b) { return len(a - b); }

// Rotate point p by theta radians CCW around origin (0,0)
Point rotate(Point p, double theta) {
  return {p.x * cos(theta) - p.y * sin(theta),
          p.x * sin(theta) + p.y * cos(theta)};
}

// Rotate point p by theta radians CCW around pivot point
Point rotateAround(Point p, double theta, Point pivot) {
  return pivot + rotate(p - pivot, theta);
}

// Orientation of ordered triplet (a, b, c)
// Returns: 0 -> Collinear, 1 -> Clockwise (Right), 2 -> Counter-Clockwise
// (Left) Note: Adjusted return values to standard convention (often useful for
// Convex Hull)
int orientation(Point a, Point b, Point c) {
  double val = cross(b - a, c - a);
  if (abs(val) < EPS)
    return 0;               // Collinear
  return (val < 0) ? 1 : 2; // 1: CW (Right), 2: CCW (Left)
}

// Get angle of vector 'a' in radians [-PI, PI]
double angle(Point a) { return atan2(a.y, a.x); }

// Normalize a vector to length 1 (Unit Vector)
Point normalize(Point v) {
  double l = len(v);
  if (dcmp(l, 0) == 0)
    return {0, 0}; // Handle zero vector
  return v / l;
}

// =========================================================
// 4. Line, Segment, Ray Tools
// =========================================================

// --- 4.1 Projections & Reflections ---

// Project point P onto line defined by AB (infinite line)
// Formula: A + dot(AP, AB) / dot(AB, AB) * AB
Point projectOnLine(Point p, Point a, Point b) {
  Point ab = b - a;
  Point ap = p - a;
  return a + ab * (dot(ap, ab) / dot(ab, ab));
}

// Reflect point P across line AB
Point reflectOverLine(Point p, Point a, Point b) {
  Point proj = projectOnLine(p, a, b);
  return proj * 2 - p;
}

// --- 4.2 Distances ---

// Minimum distance from Point P to Line AB (infinite)
double distPointToLine(Point p, Point a, Point b) {
  return abs(cross(b - a, p - a)) / dist(a, b);
}

// Minimum distance from Point P to Segment AB
// If projection falls outside segment, returns dist to closest endpoint
double distPointToSegment(Point p, Point a, Point b) {
  double l2 = dot(b - a, b - a); // Length squared of segment AB

  // Handle degenerate segment (single point)
  if (l2 < EPS)
    return dist(p, a);

  Point ab = b - a;
  Point ap = p - a;
  double t = dot(ap, ab) / l2;

  if (t < 0.0)
    return dist(p, a); // Closer to A
  if (t > 1.0)
    return dist(p, b); // Closer to B

  // Projection falls on the segment
  return distPointToLine(p, a, b);
}

// Minimum distance from Point P to Ray AB (starts at A, goes through B)
double distPointToRay(Point p, Point a, Point b) {
  Point ab = b - a;
  Point ap = p - a;
  double t = dot(ap, ab) / dot(ab, ab);

  if (t < 0.0)
    return dist(p, a);             // Behind ray start
  return distPointToLine(p, a, b); // Projection is on the ray
}

// Distance between two segments P1Q1 and P2Q2
// Returns 0 if they intersect. Else min dist between endpoints and opposite
// segments. (Requires checkIntersection to be perfectly robust, but this is the
// geometric logic) Forward declaration of intersection check needed for perfect
// 0. Simplified version (logic only): min(distPointToSeg(p1, p2, q2),
// distPointToSeg(q1, p2, q2),
//     distPointToSeg(p2, p1, q1), distPointToSeg(q2, p1, q1))
// *BUT* check for intersection first!

// --- 4.3 Intersections ---

// Check if point Q lies on segment PR
bool onSegment(Point p, Point q, Point r) {
  return q.x >= min(p.x, r.x) && q.x <= max(p.x, r.x) && q.y >= min(p.y, r.y) &&
         q.y <= max(p.y, r.y) && orientation(p, q, r) == 0;
}

/**
 * Intersection of Infinite Lines AB and CD
 * Returns:
 * 0 -> Lines are parallel and distinct (NONE)
 * 1 -> Lines are collinear/overlapping (LINE)
 * 2 -> Lines intersect at a unique point (POINT), stored in `res`
 */
int getLineIntersection(Point a, Point b, Point c, Point d, Point &res) {
  Point v1 = b - a; // Direction vector of Line 1
  Point v2 = d - c; // Direction vector of Line 2

  // Cross product of directions determines if lines are parallel
  double det = cross(v1, v2);

  // Case: Parallel or Collinear (det is approx 0)
  if (dcmp(det, 0) == 0) {
    // To distinguish, check if point C lies on Line AB.
    // We do this by checking if vector (c-a) is parallel to v1.
    if (dcmp(cross(v1, c - a), 0) == 0) {
      return 1; // LINE (Collinear)
    }
    return 0; // NONE (Parallel distinct)
  }

  // Case: Single Intersection Point
  // We solve for t in the equation: P = a + t*v1
  // Using Cramer's rule/geometry: t = cross(c - a, v2) / cross(v1, v2)
  double t = cross(c - a, v2) / det;
  res = a + v1 * t;

  return 2; // POINT
}

// Intersection of Segments AB and CD
// Returns true if they intersect properly (or touch).
bool doSegmentsIntersect(Point a, Point b, Point c, Point d) {
  int o1 = orientation(a, b, c);
  int o2 = orientation(a, b, d);
  int o3 = orientation(c, d, a);
  int o4 = orientation(c, d, b);

  // General Case
  if (o1 != o2 && o3 != o4)
    return true;

  // Special Cases (Collinear handling)
  if (o1 == 0 && onSegment(a, c, b))
    return true;
  if (o2 == 0 && onSegment(a, d, b))
    return true;
  if (o3 == 0 && onSegment(c, a, d))
    return true;
  if (o4 == 0 && onSegment(c, b, d))
    return true;

  return false;
}

// Get Intersection Points of Segments AB and CD
// Returns:
// 0 points -> No intersection
// 1 point  -> Single intersection (crossing or touching)
// 2 points -> Collinear overlap (returns endpoints of the overlapping segment)
vector<Point> getSegmentIntersections(Point a, Point b, Point c, Point d) {
  vector<Point> res;

  // Line AB represented as A + t(B-A)
  // Line CD represented as C + u(D-C)
  // Intersection happens where A + t(B-A) = C + u(D-C)

  Point ab = b - a;
  Point cd = d - c;
  Point ac = c - a;

  double det = cross(ab, cd);

  // Case 1: Lines are Parallel (det is approx 0)
  if (abs(det) < EPS) {
    // Check if they are collinear. If cross(AB, AC) != 0, they are parallel but
    // distinct.
    if (abs(cross(ab, ac)) > EPS)
      return res; // Parallel, no intersection

    // They are collinear. Find the overlap.
    // We project the segments onto the x-axis (or y-axis if vertical) and find
    // interval intersection. A simpler way using Point operator<:
    Point p1 = min(a, b), p2 = max(a, b);
    Point p3 = min(c, d), p4 = max(c, d);

    Point start = max(p1, p3);
    Point end = min(p2, p4);

    if (start < end) {
      if (dist(start, end) > EPS) {
        res.push_back(end); // Segment overlap
      }
    } else if (start == end) {
      res.push_back(start); // Touch at single point
    }
    return res;
  }

  // Case 2: Lines are not parallel (Single intersection point on infinite
  // lines) We need to check if this point lies within both segments. t =
  // cross(AC, CD) / cross(AB, CD) u = cross(AC, AB) / cross(AB, CD) -- Note:
  // check sign/order carefully

  double t = cross(ac, cd) / det;
  double u = cross(ac, ab) / det;

  // Check if the intersection lies within the segments [0, 1]
  // Use EPS for precision tolerance on boundaries
  // Check if the intersection lies within the segments [0, 1]
  if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS) {
    // CRITICAL FIX: Clamp t to [0, 1] to prevent precision drift
    if (t < 0.0)
      t = 0.0;
    if (t > 1.0)
      t = 1.0;
    res.push_back(a + ab * t);
  }

  return res;
}

// --- 4.4 Advanced Distance (Segment to Segment) ---
double distSegmentToSegment(Point p1, Point q1, Point p2, Point q2) {
  if (doSegmentsIntersect(p1, q1, p2, q2))
    return 0.0;

  double d1 = distPointToSegment(p1, p2, q2);
  double d2 = distPointToSegment(q1, p2, q2);
  double d3 = distPointToSegment(p2, p1, q1);
  double d4 = distPointToSegment(q2, p1, q1);

  return min({d1, d2, d3, d4});
}

// Helper: Check if two Rays intersect (Crossing only)
// Returns true if they cross. Parallel/Collinear handled by distance check.
bool checkRayIntersection(Point a, Point b, Point c, Point d) {
  Point ab = b - a;
  Point cd = d - c;
  Point ac = c - a;
  double det = cross(ab, cd);

  // If parallel (det is 0), they don't "cross" in a way that requires solving.
  // Overlapping parallel rays are handled by the distPointToRay check returning
  // 0.
  if (dcmp(det, 0) == 0)
    return false;

  // Solve P1 + t*V1 = P3 + u*V2
  double t = cross(ac, cd) / det;
  double u = cross(ac, ab) / det;

  // t >= 0 means intersection is on Ray 1
  // u >= 0 means intersection is on Ray 2
  return (dcmp(t, 0) >= 0 && dcmp(u, 0) >= 0);
}

// MAIN FUNCTION: Distance between Ray (a->b) and Ray (c->d)
double distRayToRay(Point p1, Point p2, Point p3, Point p4) {
  // Case 1: If they strictly intersect, distance is 0
  if (checkRayIntersection(p1, p2, p3, p4)) {
    return 0.0;
  }

  // Case 2: If no intersection, the closest distance MUST involve an endpoint.
  // It's the minimum of:
  // A) Distance from Ray1's Start (p1) -> to Ray2
  double d1 = distPointToRay(p1, p3, p4);

  // B) Distance from Ray2's Start (p3) -> to Ray1
  double d2 = distPointToRay(p3, p1, p2);

  return min(d1, d2);
}

// =========================================================
// 5. Polygons
// =========================================================

// Area of Polygon (Shoelace Formula)
// Vertices can be CW or CCW
double polygonArea(const vector<Point> &points) {
  double area = 0.0;
  int n = points.size();
  for (int i = 0; i < n; i++) {
    area += cross(points[i], points[(i + 1) % n]);
  }
  return abs(area) / 2.0;
}

// Check if Point P is inside Polygon
// Returns: 1 (Inside), 0 (Boundary), -1 (Outside)
// Uses Ray Casting Algorithm (Horizontal ray to the right)
int pointInPolygon(const vector<Point> &poly, Point p) {
  int n = poly.size();
  bool inside = false;
  for (int i = 0, j = n - 1; i < n; j = i++) {
    // Check if point is on boundary (segment j-i)
    if (onSegment(poly[j], p, poly[i]))
      return 0;

    // Ray casting logic
    // Check if ray crosses the edge (poly[j], poly[i])
    // 1. One point below p.y, one point above or equal
    // 2. Point is strictly to the left of the intersection
    if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
        (p.x <
         (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) +
             poly[i].x)) {
      inside = !inside;
    }
  }
  return inside ? 1 : -1;
}

// Convex Hull (Monotone Chain Algorithm)
// Returns points of the hull in CCW order
// Warning: Handles duplicate points; Remove them beforehand if needed.
vector<Point> convexHull(vector<Point> &pts) {
  int n = pts.size(), k = 0;
  if (n <= 2)
    return pts;
  vector<Point> h(2 * n);

  // Sort points lexicographically
  sort(pts.begin(), pts.end());

  // Build lower hull
  for (int i = 0; i < n; ++i) {
    while (k >= 2 && cross(h[k - 1] - h[k - 2], pts[i] - h[k - 1]) <= 0)
      k--;
    h[k++] = pts[i];
  }

  // Build upper hull
  for (int i = n - 2, t = k + 1; i >= 0; i--) {
    while (k >= t && cross(h[k - 1] - h[k - 2], pts[i] - h[k - 1]) <= 0)
      k--;
    h[k++] = pts[i];
  }

  h.resize(k - 1); // Remove duplicate of the start point
  return h;
}

// 1.1 Simplify Vector (Canonical Form)
// Divides x and y by their GCD to get the smallest integer representation.
// Ensures the vector points in a "canonical" direction (e.g., first non-zero
// component is positive). Useful for map keys (map<Point, int> counts) or
// unique slope representation.
long long gcd(long long a, long long b) { return b == 0 ? a : gcd(b, a % b); }

Point simplify(Point p) {
  long long x = (long long)round(p.x); // Assuming integer coords for GCD
  long long y = (long long)round(p.y);

  if (x == 0 && y == 0)
    return {0, 0};

  long long common = gcd(abs(x), abs(y));
  x /= common;
  y /= common;

  // Canonical sign enforcement:
  // Ensure first non-zero component is positive
  if (x < 0 || (x == 0 && y < 0)) {
    x = -x;
    y = -y;
  }
  return {(double)x, (double)y};
}

// 1.2 Get Normal Vector (Perpendicular)
// Returns a vector perpendicular to v.
// Rotates 90 degrees CCW: (x, y) -> (-y, x)
Point perp(Point p) { return {-p.y, p.x}; }
// 2.1 Points to Line Equation (Ax + By + C = 0)
struct Line {
  double a, b, c;
};

// Returns {A, B, C} for line passing through P and Q
Line getLineEquation(Point p, Point q) {
  double a = p.y - q.y;
  double b = q.x - p.x;
  double c = -a * p.x - b * p.y;

  // Optional: Normalize A, B, C for uniqueness if dealing with integers
  // Requires converting to long long and using GCD logic similar to 'simplify'
  return {a, b, c};
}

// 2.2 Distance from Point to Line (using Equation)
// Formula: |Ax + By + C| / sqrt(A^2 + B^2)
double distToLine(Point p, Line l) {
  return abs(l.a * p.x + l.b * p.y + l.c) / sqrt(l.a * l.a + l.b * l.b);
}

// 2.3 Intersection of Lines (using Equation)
// Solves system of linear equations
bool areParallel(Line l1, Line l2) {
  return abs(l1.a * l2.b - l1.b * l2.a) < EPS;
}

Point intersectLines(Line l1, Line l2) {
  double det = l1.a * l2.b - l2.a * l1.b;
  if (abs(det) < EPS)
    return {INF, INF}; // Parallel lines
  double x = (l1.b * l2.c - l2.b * l1.c) / det;
  double y = (l1.c * l2.a - l2.c * l1.a) / det;
  return {x, y};
}
// 3.1 Lattice Points on Segment
// Returns number of integer points on segment AB (excluding endpoints? No,
// includes them) Formula: GCD(|dx|, |dy|) + 1
long long latticePointsOnSegment(Point a, Point b) {
  long long dx = abs((long long)round(a.x - b.x));
  long long dy = abs((long long)round(a.y - b.y));
  return gcd(dx, dy) + 1;
}

// 3.2 Polar Sort (Advanced Sorting)
// Sorts points angularly around a center C.
// Useful for: Convex Hull, angular sweep lines.
Point center; // Set this global or pass as lambda capture
bool polarCmp(Point a, Point b) {
  if (a.x - center.x >= 0 && b.x - center.x < 0)
    return true;
  if (a.x - center.x < 0 && b.x - center.x >= 0)
    return false;
  if (a.x - center.x == 0 && b.x - center.x == 0) {
    if (a.y - center.y >= 0 || b.y - center.y >= 0)
      return a.y > b.y;
    return b.y > a.y;
  }

  // Compute cross product relative to center
  double det = cross(a - center, b - center);
  if (det < 0)
    return false;
  if (det > 0)
    return true;

  // Collinear: closer point first? or further? Depends on problem.
  return dist(a, center) < dist(b, center);
}

// 3.3 Angle Bisector
// Returns a vector direction that bisects angle A-O-B
Point angleBisector(Point a, Point o, Point b) {
  Point oa = a - o;
  Point ob = b - o;
  // Normalize both vectors to length 1
  return normalize(oa) + normalize(ob);
}

// Given a polygon with integer coordinates:
// Area = InsidePoints + BoundaryPoints/2 - 1
// Returns number of integer points strictly inside the polygon
long long countInteriorPoints(const vector<Point> &poly) {
  double area = polygonArea(poly); // From previous snippet
  long long boundaryPoints = 0;
  int n = poly.size();
  for (int i = 0; i < n; i++) {
    boundaryPoints += latticePointsOnSegment(poly[i], poly[(i + 1) % n]) - 1;
  }
  // Invert Pick's Theorem: Inside = Area - Boundary/2 + 1
  return (long long)area - boundaryPoints / 2 + 1;
}

// Convert two points to a Line (ax + by + c = 0)
Line pointsToLine(Point p1, Point p2) {
  if (dcmp(p1.x, p2.x) == 0) { // Vertical Line
    return {1.0, 0.0, -p1.x};
  }
  double a = -(p1.y - p2.y);
  double b = (p1.x - p2.x);
  double c = -a * p1.x - b * p1.y;
  return {a, b, c};
}

// Compute Determinant of 2x2 matrix
// | a  b |
// | c  d |
double det(double a, double b, double c, double d) { return a * d - b * c; }

// Intersection of two lines using Cramer's Rule
// Returns true if intersection exists, false if parallel
bool intersectLineCramer(Line l1, Line l2, Point &res) {
  // Main determinant (D)
  double D = det(l1.a, l1.b, l2.a, l2.b);

  // If D is 0, lines are parallel or identical
  if (dcmp(D, 0) == 0)
    return false;

  // Determinants for X and Y (Dx, Dy)
  // Note: Constants C are on LHS in our struct (Ax + By + C = 0),
  // but Cramer's assumes Ax + By = -C.
  // So we use -l1.c and -l2.c
  double Dx = det(-l1.c, l1.b, -l2.c, l2.b);
  double Dy = det(l1.a, -l1.c, l2.a, -l2.c);

  res.x = Dx / D;
  res.y = Dy / D;
  return true;
}

// HOW TO USE THIS

// // Line 1: Passing through (0,0) and (4,4) -> x - y = 0
// Line l1 = pointsToLine({0, 0}, {4, 4});

// // Line 2: Passing through (0,4) and (4,0) -> x + y - 4 = 0
// Line l2 = pointsToLine({0, 4}, {4, 0});

// Point intersection;
// if (intersectLineCramer(l1, l2, intersection)) {
//   cout << "Intersection: " << intersection.x << ", " << intersection.y <<
//   endl;
// } else {
//   cout << "Lines are parallel" << endl;
// }
// // Output should be: Intersection: 2, 2

// Circle

// Circle structure to store center and radius
struct Circle {
  Point center;
  double r;
};

// Function to get a circle defined by 3 non-collinear points
// Uses the determinant method (Cramer's rule) for stability and ease
// For only Radius R
// r = (a*b*c)/(4*areaOf(a,b,c)) (a,b,c = length of triangle)
Circle getCircleFrom3Points(Point A, Point B, Point C) {
  double x1 = A.x, y1 = A.y;
  double x2 = B.x, y2 = B.y;
  double x3 = C.x, y3 = C.y;

  // D = 2 * (x1(y2 - y3) + x2(y3 - y1) + x3(y1 - y2))
  double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

  // Calculate Center X (Ux / D)
  double Ux =
      ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) +
       (x3 * x3 + y3 * y3) * (y1 - y2));

  // Calculate Center Y (Uy / D)
  double Uy =
      ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) +
       (x3 * x3 + y3 * y3) * (x2 - x1));

  Point center = {Ux / D, Uy / D};

  // Radius = distance from center to any point (e.g., A)
  double radius = sqrt(pow(center.x - x1, 2) + pow(center.y - y1, 2));

  return {center, radius};
}

// =========================================================
// 6. Advanced Circle Operations
// =========================================================

// 6.1 Basics & Point Position
// ---------------------------------------------------------

// Check point position relative to circle
// Returns: 0 -> Inside, 1 -> On Boundary, 2 -> Outside
int checkPointPosition(Circle c, Point p) {
    double d = dist(c.center, p);
    if (dcmp(d, c.r) == 0) return 1; // On boundary
    if (d < c.r) return 0;           // Inside
    return 2;                        // Outside
}

// 6.2 Line / Segment / Ray Intersections with Circle
// ---------------------------------------------------------

/**
 * Intersection of Infinite Line AB and Circle C
 * Logic:
 * 1. Find projection of Center onto Line AB.
 * 2. Calculate distance (h) from Center to Line.
 * 3. If h > r: No intersection.
 * 4. If h = r: Tangent (1 point).
 * 5. If h < r: Secant (2 points). Points are offset from projection.
 */
vector<Point> intersectLineCircle(Point a, Point b, Circle c) {
    vector<Point> res;
    // Special case: A and B are same
    if(a == b) return res; 

    Point ab = b - a;
    // Projection of center onto infinite line AB
    Point p = a + ab * (dot(c.center - a, ab) / dot(ab, ab));
    
    double h2 = dot(p - c.center, p - c.center); // Distance squared
    double r2 = c.r * c.r;

    // No intersection
    if (dcmp(h2, r2) > 0) return res;

    // Tangent (1 point) or Secant (2 points)
    // Distance from projection point P to intersection points
    double offset = sqrt(max(0.0, r2 - h2)); 
    Point dir = normalize(ab); // Unit vector of line

    res.push_back(p - dir * offset);
    if (dcmp(offset, 0) > 0) { // If not tangent, add second point
        res.push_back(p + dir * offset);
    }
    return res;
}

// Intersection of Segment AB and Circle
// Calls line intersection, then checks if points are on segment.
vector<Point> intersectSegmentCircle(Point a, Point b, Circle c) {
    vector<Point> linePoints = intersectLineCircle(a, b, c);
    vector<Point> res;
    for (auto p : linePoints) {
        if (onSegment(a, p, b)) { // Uses your existing onSegment function
            res.push_back(p);
        }
    }
    return res;
}

// 6.3 Circle - Circle Relations
// ---------------------------------------------------------

/**
 * Intersection of Two Circles (c1 and c2)
 * Logic: Uses Law of Cosines on the triangle formed by centers and intersection point.
 * d = distance between centers.
 * a = (r1^2 - r2^2 + d^2) / 2d  (Distance from c1 along the connecting line)
 * h = sqrt(r1^2 - a^2)          (Perpendicular distance from connecting line)
 */
vector<Point> intersectCircleCircle(Circle c1, Circle c2) {
    vector<Point> res;
    double d = dist(c1.center, c2.center);

    // Case 0: Same circle (Infinite points) - Handled as no unique points here
    if (dcmp(d, 0) == 0 && dcmp(c1.r, c2.r) == 0) return res; 
    
    // Case 1: Too far apart (d > r1 + r2) or One inside other (d < |r1-r2|)
    if (dcmp(d, c1.r + c2.r) > 0 || dcmp(d, abs(c1.r - c2.r)) < 0) return res;

    // Calculate distance 'a' from c1 center to the perpendicular line of intersection
    double a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2.0 * d);
    
    // Calculate height 'h' (perpendicular offset)
    double h = sqrt(max(0.0, c1.r * c1.r - a * a));

    // Calculate P2 (point on the line connecting centers)
    Point p2 = c1.center + (c2.center - c1.center) * (a / d);
    
    // Calculate intersection points: P2 +/- h * (Perpendicular Vector)
    double x2 = c2.center.x - c1.center.x;
    double y2 = c2.center.y - c1.center.y;
    
    res.push_back({p2.x - h * y2 / d, p2.y + h * x2 / d});
    
    // If not touching at a single point, add the second one
    if (dcmp(h, 0) > 0) {
        res.push_back({p2.x + h * y2 / d, p2.y - h * x2 / d});
    }
    return res;
}

// 6.4 Tangents
// ---------------------------------------------------------

// Tangents from Point P to Circle C
// Returns tangent points on the circle.
// Logic: Triangle formed by Center, P, and TangentPoint is Right Angled.
vector<Point> tangentsPointCircle(Point p, Circle c) {
    vector<Point> res;
    double d = dist(p, c.center);

    // Point is inside circle -> No tangent
    if (d < c.r - EPS) return res;

    // Point is on circle -> 1 tangent (the point itself)
    if (abs(d - c.r) < EPS) {
        res.push_back(p);
        return res;
    }

    // Point is outside -> 2 tangents
    // Angle alpha = asin(r / d)
    double alpha = asin(c.r / d);
    // Angle theta = angle of vector CP
    double theta = atan2(p.y - c.center.y, p.x - c.center.x);

    // Two tangent points are at angles theta + alpha and theta - alpha
    res.push_back(c.center + Point(c.r * cos(theta - alpha), c.r * sin(theta - alpha)));
    res.push_back(c.center + Point(c.r * cos(theta + alpha), c.r * sin(theta + alpha)));
    return res;
}

// Common Tangents between Two Circles
// (Advanced: Useful for "Belt" problems or convex hull of circles)
// Returns list of lines (pairs of points on c1 and c2).
// Not implemented fully here due to length, but relies on:
// External Tangents: construct lines parallel to center-center line offset by (r1-r2)
// Internal Tangents: offset by (r1+r2)


// 6.5 Areas
// ---------------------------------------------------------

// Intersection Area of Two Circles
double circleIntersectionArea(Circle c1, Circle c2) {
    double d = dist(c1.center, c2.center);

    // Case 1: Too far apart
    if (d >= c1.r + c2.r) return 0.0;

    // Case 2: One completely inside the other
    if (d <= abs(c1.r - c2.r)) {
        double r = min(c1.r, c2.r);
        return PI * r * r;
    }

    // Case 3: Standard intersection
    // Area = Area of Sector 1 + Area of Sector 2 - Area of Kite (Triangle part)
    // Formula using cosine rule angles:
    double ang1 = 2.0 * acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2.0 * c1.r * d));
    double ang2 = 2.0 * acos((c2.r * c2.r + d * d - c1.r * c1.r) / (2.0 * c2.r * d));

    // Area of circular segment = (r^2 / 2) * (theta - sin(theta))
    double area1 = 0.5 * c1.r * c1.r * (ang1 - sin(ang1));
    double area2 = 0.5 * c2.r * c2.r * (ang2 - sin(ang2));

    return area1 + area2;
}

// 6.6 Useful Geometric Formulas (Sectors, Chords)
// ---------------------------------------------------------

// Angle subtended by chord of length 'len' at center
double angleFromChord(double r, double chordLen) {
    return 2.0 * asin(chordLen / (2.0 * r));
}

// Length of chord given angle theta (radians)
double chordLength(double r, double theta) {
    return 2.0 * r * sin(theta / 2.0);
}

// Area of Circular Sector (Pie slice)
double sectorArea(double r, double theta) {
    return 0.5 * r * r * theta;
}

// Area of Circular Segment (Cut off by chord)
// theta is the angle subtended by the chord at the center
double segmentArea(double r, double theta) {
    return 0.5 * r * r * (theta - sin(theta));
}

// =========================================================
// 7. Advanced Construction & Tangents (Commented)
// =========================================================

// 7.1 Circle from 2 Points and Radius
// ---------------------------------------------------------
// PROBLEM: Find the center(s) of a circle of radius 'r' that passes through points P1 and P2.
// LOGIC:
// 1. The centers must lie on the perpendicular bisector of the segment P1-P2.
// 2. We form a right triangle with:
//    - Hypotenuse = r (radius)
//    - Base = half the distance between P1 and P2 (d/2)
//    - Height 'h' = distance from the midpoint of P1-P2 to the circle center.
// 3. Pythagoras: h^2 + (d/2)^2 = r^2  =>  h = sqrt(r^2 - d^2/4)
vector<Point> getCircleCenter(Point p1, Point p2, double r) {
    vector<Point> res;
    
    // Squared distance between p1 and p2
    double d2 = magSq(p1 - p2); 
    
    // determinant = h^2 = r^2 - (d/2)^2 = r^2 - d^2/4
    double det = r * r - d2 / 4.0;

    // If det < 0, then (d/2)^2 > r^2, meaning the points are too far apart 
    // to be covered by a circle of radius 'r'.
    if (det < 0.0) return res; 

    // h is the distance from the midpoint to the center
    double h = sqrt(det);
    
    // Find the Midpoint of P1 and P2
    Point mid = (p1 + p2) / 2.0;
    
    // Calculate the Perpendicular Vector to P1->P2
    // 1. Get vector P1->P2
    Point ab = p2 - p1;
    // 2. Rotate 90 degrees: (x, y) -> (-y, x). 
    //    We also normalize it by dividing by length (sqrt(d2))? 
    //    Actually, simpler: Just normalize the direction then scale by h.
    //    Here we use the property: perp = {-y, x}.
    //    Note: The code below effectively does (UnitPerp * h).
    //    Point perp = {-ab.y, ab.x} is length 'd'. We need length 'h'.
    //    So we multiply by h / d. Or normalize first.
    //    Let's stick to the code logic:
    
    Point perp = {-ab.y, ab.x}; // Vector of length 'd' perpendicular to ab
    
    // We normalize 'perp' to length 1, then multiply by 'h'.
    // perp / sqrt(d2) is unit vector.
    // So shift = perp * (h / sqrt(d2)).
    Point shift = perp * (h / sqrt(d2));

    // Two possible centers: Midpoint + shift, Midpoint - shift
    res.push_back(mid + shift);
    res.push_back(mid - shift);
    return res;
}

// 7.2 Incircle of a Triangle
// ---------------------------------------------------------
// PROBLEM: Find the circle inscribed inside Triangle ABC.
// LOGIC:
// Center (Incenter): The weighted average of vertices, where weights are the lengths of opposite sides.
// Formula: I = (a*A + b*B + c*C) / (a + b + c)
// Radius (Inradius): Area of Triangle / Semiperimeter
Circle getIncircle(Point a, Point b, Point c) {
    // Calculate side lengths
    double side_a = dist(b, c); // Side opposite Vertex A
    double side_b = dist(a, c); // Side opposite Vertex B
    double side_c = dist(a, b); // Side opposite Vertex C
    
    double p = side_a + side_b + side_c; // Perimeter
    double s = p / 2.0;                  // Semiperimeter (used for radius)
    
    // Incenter Formula: Weighted sum of coordinates
    double x = (side_a * a.x + side_b * b.x + side_c * c.x) / p;
    double y = (side_a * a.y + side_b * b.y + side_c * c.y) / p;
    
    // Inradius Formula: r = Area / s
    // Area is calculated using Cross Product (2 * Area = |cross|)
    double area = abs(cross(b - a, c - a)) / 2.0;
    double r = area / s;

    return {{x, y}, r};
}

// 7.3 Common Tangents (External & Internal)
// ---------------------------------------------------------
// PROBLEM: Find lines tangent to two circles C1 and C2.
// RETURNS: Pairs of points {P1, P2} where P1 is on C1, P2 is on C2.
// LOGIC:
// We use the "Homothetic Center" or simple rotation logic.
// Imagine the radius C1->P1. It is perpendicular to the tangent line.
// We find the angle of this radius vector relative to the center-to-center line.
// Construct a right triangle with hypotenuse = dist(centers) and leg = |r1 - r2| (external) or r1 + r2 (internal).
vector<pair<Point, Point>> getCommonTangents(Circle c1, Circle c2) {
    vector<pair<Point, Point>> lines;
    
    Point d = c2.center - c1.center; // Vector from C1 to C2
    double r = c1.r, R = c2.r;
    double distSq = magSq(d);
    double dist = sqrt(distSq); // Distance between centers

    if (distSq < EPS) return lines; // Concentric circles -> No unique tangents

    // Note: The lambda 'add_tangent' in the original snippet was unused.
    // We proceed with the manual calculation blocks below.

    // -------------------------------------------------
    // TYPE 1: External Tangents (Outer belts)
    // -------------------------------------------------
    // Condition: Tangents exist if circles are not one-inside-other.
    // Math: |r - R| <= distance
    // Logic: Construct a Right Triangle C1-X-C2.
    //        Hypotenuse = dist. Leg = |R - r|.
    //        Angle alpha at C1 = acos(Leg / Hypotenuse).
    if (distSq >= (r - R) * (r - R)) {
        // Calculate angle offset 'alpha' relative to the center-center line
        // cos(alpha) = adj / hyp = |R - r| / dist
        double alpha = acos(abs(r - R) / dist);
        
        // We need to rotate the normalized direction vector 'd' by +alpha and -alpha
        // This gives the direction of the RADIUS to the tangent point.
        Point dir = normalize(d);
        
        // Tangent 1 (Top side)
        // Point on C1: Center + Radius * rotated_direction
        Point p1a = c1.center + rotate(dir, alpha) * r;
        // Point on C2: Center + Radius * rotated_direction
        // Note: For external tangents, radii are parallel, so we use same angle 'alpha'
        Point p1b = c2.center + rotate(dir, alpha) * R;
        lines.push_back({p1a, p1b});

        // Tangent 2 (Bottom side)
        // Rotate by -alpha
        Point p2a = c1.center + rotate(dir, -alpha) * r;
        Point p2b = c2.center + rotate(dir, -alpha) * R;
        lines.push_back({p2a, p2b});
    }

    // -------------------------------------------------
    // TYPE 2: Internal Tangents (Crossing belts)
    // -------------------------------------------------
    // Condition: Tangents exist if circles are separated.
    // Math: r + R <= distance
    // Logic: Right Triangle with Hypotenuse = dist, Leg = r + R.
    //        cos(alpha) = (r + R) / dist
    if (distSq >= (r + R) * (r + R)) {
        // Calculate angle offset 'alpha'
        double alpha = acos((r + R) / dist);
        Point dir = normalize(d);

        // Tangent 3 (Crosses top-to-bottom)
        // Point on C1: Rotated by +alpha
        Point p3a = c1.center + rotate(dir, alpha) * r;
        // Point on C2: Rotated by +alpha + PI (180 degrees)
        // Why +PI? Because internal tangents cross; the radius on C2 points 
        // in the opposite direction to the radius on C1.
        Point p3b = c2.center + rotate(dir, alpha + PI) * R;
        lines.push_back({p3a, p3b});

        // Tangent 4 (Crosses bottom-to-top)
        // Point on C1: Rotated by -alpha
        Point p4a = c1.center + rotate(dir, -alpha) * r;
        // Point on C2: Rotated by -alpha + PI
        Point p4b = c2.center + rotate(dir, -alpha + PI) * R;
        lines.push_back({p4a, p4b});
    }
    
    return lines;
}


// =========================================================
// 8. Advanced Area: Circle-Polygon Intersection (Commented)
// =========================================================

// HELPER: Signed Area of a Circular Sector
// Logic: Calculates the area swept by the arc from angle of A to angle of B.
// This is NOT the standard sector area (0.5*r*r*theta) but a SIGNED version
// that works even if the origin is outside the polygon.
double sectorArea_signed(Point a, Point b, double r) {
    // Calculate angle of point B relative to x-axis
    double thetaB = atan2(b.y, b.x); 
    // Calculate angle of point A relative to x-axis
    double thetaA = atan2(a.y, a.x); 
    
    // Calculate the difference (angle subtended by arc AB)
    double theta = thetaB - thetaA;

    // Normalize angle to be within range (-PI, PI]
    // This handles cases where the arc crosses the -180/180 degree line.
    while (theta <= -PI) theta += 2 * PI;
    while (theta > PI) theta -= 2 * PI;
    
    // Formula: 1/2 * r^2 * theta
    return 0.5 * r * r * theta;
}

// MAIN FUNCTION
// Logic: Iterates through all edges, shifting coordinate system so Circle Center is (0,0).
// Splits edges based on intersection with the circle boundary.
double getCirclePolygonArea(Circle c, vector<Point> poly) {
    double area = 0.0; // Accumulator for total signed area
    int n = poly.size(); // Number of vertices

    for (int i = 0; i < n; i++) {
        // Shift Coordinate System:
        // We act as if the circle center is at (0,0).
        // Point 'a' is the current vertex, 'b' is the next vertex.
        Point a = poly[i] - c.center;       
        Point b = poly[(i + 1) % n] - c.center;
        
        // Check if endpoints are strictly inside the circle.
        // We use magnitude squared (x^2 + y^2) compared to r^2 to avoid slow sqrt().
        bool inA = dcmp(magSq(a), c.r * c.r) <= 0;
        bool inB = dcmp(magSq(b), c.r * c.r) <= 0;
        
        // CASE 1: Both endpoints are inside the circle.
        // The edge AB is fully inside. The area is just the triangle OAB.
        if (inA && inB) {
            // Signed area of triangle OAB = Cross Product / 2
            area += cross(a, b) / 2.0;
        } 
        // CASE 2: At least one point is outside.
        else {
            // Find intersection points of Segment AB with the Circle.
            // We use a dummy circle at (0,0) because we shifted points A and B.
            vector<Point> ints = intersectSegmentCircle(a, b, {{0,0}, c.r});
            
            // SUB-CASE 2a: No intersection with the segment.
            // This means the segment is fully outside (or touches but doesn't cross in a way that matters).
            // The area contribution is purely the circular sector swept by A->B.
            if (ints.empty()) {
                area += sectorArea_signed(a, b, c.r);
            } 
            // SUB-CASE 2b: The segment intersects the circle boundary.
            else {
                // Sorting is CRITICAL.
                // We need to process the path from A -> B in order.
                // If there are 2 intersections, we must hit the closer one first.
                // If ints[0] is further from A than ints[1], swap them.
                if (ints.size() > 1 && dist(a, ints[0]) > dist(a, ints[1])) 
                    swap(ints[0], ints[1]);
                
                // Now we traverse the path A -> Int1 -> (Int2) -> B
                // We sum the area of each sub-segment.

                // PART 1: From Start Point A -> First Intersection
                if (inA) {
                    // If A is inside, the path A->Int1 is a straight line segment inside the circle.
                    // Contribution: Triangle Area (O, A, Int1)
                    area += cross(a, ints[0]) / 2.0;
                } else {
                    // If A is outside, the path A->Int1 is "hovering" over the sector.
                    // Contribution: Sector Area (O, A, Int1)
                    area += sectorArea_signed(a, ints[0], c.r);
                }
                
                // PART 2: Handling the middle or end
                if (ints.size() == 2) {
                    // If there are 2 intersections, the segment 'Int1 -> Int2' is definitely INSIDE.
                    // Because a line entering a circle must be inside until it exits.
                    // Contribution: Triangle Area (O, Int1, Int2)
                    area += cross(ints[0], ints[1]) / 2.0;
                    
                    // PART 3: From Second Intersection -> End Point B
                    if (inB) {
                        // If B is inside (rare with 2 ints, usually implies B is ON boundary), Triangle.
                        area += cross(ints[1], b) / 2.0;
                    } else {
                        // If B is outside, we exited the circle.
                        // Contribution: Sector Area (O, Int2, B)
                        area += sectorArea_signed(ints[1], b, c.r);
                    }
                } else {
                    // If there is only 1 intersection, we just go Int1 -> B.
                    // PART 3 (Simulated): From First Intersection -> End Point B
                    if (inB) {
                        // If B is inside, Triangle Area.
                        area += cross(ints[0], b) / 2.0;
                    } else {
                        // If B is outside, Sector Area.
                        area += sectorArea_signed(ints[0], b, c.r);
                    }
                }
            }
        }
    }
    // The result might be negative due to orientation (CW vs CCW).
    // Return absolute value for physical area.
    return abs(area);
}
// =========================================================
// 9. Circle Inversion (Commented)
// =========================================================

// Function: Invert a Point P with respect to an Inversion Circle 'inv'
// Logic: The new point P' lies on the ray from Center to P.
// The distances satisfy: dist(Center, P) * dist(Center, P') = r^2
Point invertPoint(Point p, Circle inv) {
    // Vector from Inversion Center to Point P
    Point diff = p - inv.center;
    
    // Squared distance |P - Center|^2
    double d2 = dot(diff, diff);
    
    // Special Case: The center itself inverts to Infinity.
    // We return {INF, INF} to represent this.
    if (d2 < EPS) return {INF, INF}; 

    // Formula: P' = Center + (P - Center) * (r^2 / |P - Center|^2)
    // We scale the 'diff' vector by the factor (r^2 / d^2).
    return inv.center + diff * (inv.r * inv.r / d2);
}

// Function: Invert a Circle 'c' with respect to Inversion Circle 'inv'
// Returns: A new Circle.
// RESTRICTION: This specific function assumes the result is a Circle.
// If the input circle 'c' passes through the inversion center, the result
// is actually a straight LINE, not a circle. This code does NOT handle that case.
Circle invertCircle(Circle c, Circle inv) {
    // Distance between the centers of the two circles
    double d = dist(c.center, inv.center);
    
    // We use the analytical formula for the new radius R' and center C'.
    // Denominator term derived from similarity: d^2 - R^2
    // This represents the "power of the point" of the inversion center relative to circle c.
    double denom = d*d - c.r*c.r;
    
    // Calculate the scaling ratio: r_inv^2 / (d^2 - R^2)
    double ratio = inv.r * inv.r / denom;
    
    // Calculate New Center C'
    // It lies on the line connecting the two centers.
    // Formula: C' = InvCenter + (OldCenter - InvCenter) * ratio
    Point newCenter = inv.center + (c.center - inv.center) * ratio;
    
    // Calculate New Radius R'
    // Formula: R' = | R * ratio |
    // We use abs() because 'denom' can be negative if the inversion center is inside circle c.
    double newR = abs(c.r * ratio);
    
    return {newCenter, newR};
}
// =========================================================
// 10. Minimum Enclosing Circle (Welzl's Algorithm - Commented)
// =========================================================

// Helper: Create a circle with diameter defined by 2 points
Circle circleFrom2(Point p1, Point p2) {
    Point center = (p1 + p2) / 2.0; // Midpoint
    return {center, dist(p1, p2) / 2.0}; // Radius is half distance
}

// Helper: Check if point 'p' is strictly inside or on boundary of 'c'
bool inCircle(Circle c, Point p) {
    // We use <= with EPS for numerical stability
    return dist(c.center, p) <= c.r + EPS;
}

// Recursive Welzl's Algorithm
// INPUT: 
//   P: Vector of all points to cover
//   R: Set of points KNOWN to be on the boundary of the solution circle
//   n: Number of points in P currently being considered (effectively P[0...n-1])
Circle welzl(const vector<Point>& P, vector<Point> R, int n) {
    // BASE CASES:
    // 1. n == 0: No more points to check. The circle is defined by boundary set R.
    // 2. R.size() == 3: A circle is uniquely defined by 3 boundary points.
    if (n == 0 || R.size() == 3) {
        if (R.empty()) return {{0, 0}, 0}; // Case: 0 points total
        if (R.size() == 1) return {R[0], 0}; // Case: 1 point (Radius 0)
        if (R.size() == 2) return circleFrom2(R[0], R[1]); // Case: 2 points (Diameter)
        
        // Case: 3 points (Circumcircle)
        // Ensure you have the 'getCircleFrom3Points' function from earlier in your template!
        return getCircleFrom3Points(R[0], R[1], R[2]);
    }

    // RECURSIVE STEP:
    // Pick the last point 'p' from the current subset P[0...n-1]
    Point p = P[n - 1];
    
    // Try to find the Minimum Circle for the subset WITHOUT 'p' (P[0...n-2])
    // We pass 'n - 1' to effectively ignore the current point 'p'
    Circle d = welzl(P, R, n - 1);

    // CHECK: Is the removed point 'p' inside the circle 'd' we just found?
    if (inCircle(d, p)) {
        // If yes, then 'd' is still the valid MEC for the set including 'p'.
        // We don't need to change anything.
        return d;
    }

    // If 'p' is OUTSIDE 'd', then 'p' MUST be on the boundary of the new MEC.
    // Why? Because 'd' was the optimal circle for everything else. If 'p' breaks it,
    // the circle must expand to touch 'p'.
    
    // Add 'p' to the boundary set R
    R.push_back(p);
    
    // Recurse again, but now enforcing that 'p' is on the boundary.
    return welzl(P, R, n - 1);
}

// Main Wrapper Function
Circle minEnclosingCircle(vector<Point> pts) {
    // CRITICAL STEP: Random Shuffle
    // Welzl's algorithm is O(N) *expected* time.
    // Without shuffling, a bad input order (e.g., sorted points) could trigger
    // the worst-case O(N^3) complexity or high recursion depth.
    random_shuffle(pts.begin(), pts.end());
    
    // Start recursion with empty boundary set R
    return welzl(pts, {}, pts.size());
}

// =========================================================
// 11. Advanced Polygon Operations
// =========================================================

// 11.1 Polygon Perimeter
// ---------------------------------------------------------
double polygonPerimeter(const vector<Point>& p) {
    double res = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        res += dist(p[i], p[(i + 1) % n]);
    }
    return res;
}

// 11.2 Check if a Polygon is Convex
// ---------------------------------------------------------
// Returns true if all turns are in the same direction (CW or CCW)
bool isConvex(const vector<Point>& p) {
    int n = p.size();
    if (n <= 2) return false;
    bool hasPos = false, hasNeg = false;
    for (int i = 0; i < n; i++) {
        int o = orientation(p[i], p[(i + 1) % n], p[(i + 2) % n]);
        if (o == 1) hasPos = true;
        if (o == 2) hasNeg = true;
        if (hasPos && hasNeg) return false;
    }
    return true;
}

// 11.3 Polygon Centroid (Center of Mass)
// ---------------------------------------------------------
Point polygonCentroid(const vector<Point>& p) {
    int n = p.size();
    Point c = {0, 0};
    double area = 0.0;
    for (int i = 0; i < n; i++) {
        double cross_val = cross(p[i], p[(i + 1) % n]);
        area += cross_val;
        c = c + (p[i] + p[(i + 1) % n]) * cross_val;
    }
    area /= 2.0;
    return c / (6.0 * area);
}

// 11.4 Point in Convex Polygon O(log N)
// ---------------------------------------------------------
// HIGHLY OPTIMIZED PIP: Assumes polygon is sorted strictly CCW.
// Returns: 1 (Inside), 0 (Boundary), -1 (Outside)
int pointInConvexPolygon(const vector<Point>& p, Point q) {
    int n = p.size();
    if (n < 3) return -1;
    
    // Check if outside the extreme angular range of the polygon
    if (orientation(p[0], p[1], q) == 1 || orientation(p[0], p[n - 1], q) == 2) return -1;
    if (onSegment(p[0], q, p[1]) || onSegment(p[0], q, p[n - 1])) return 0;

    // Binary search the wedges formed by p[0] and the other vertices
    int l = 1, r = n - 2;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        int dir = orientation(p[0], p[mid], q);
        if (dir == 2) l = mid + 1; // q is to the left, look in the upper half
        else r = mid - 1;          // q is to the right, look in the lower half
    }
    
    int idx = r;
    int o = orientation(p[idx], p[idx + 1], q);
    
    if (o == 2) return 1; // Strictly inside the specific wedge
    if (o == 0 && onSegment(p[idx], q, p[idx + 1])) return 0; // On the far boundary edge
    return -1;
}

// 11.5 Polygon Cut (Half-plane intersection for a single line)
// ---------------------------------------------------------
// Cuts a convex polygon by a directed line A->B.
// Returns the new polygon formed by points to the LEFT of A->B.
vector<Point> polygonCut(const vector<Point>& p, Point a, Point b) {
    vector<Point> res;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        Point p1 = p[i], p2 = p[(i + 1) % n];
        int left1 = orientation(a, b, p1);
        int left2 = orientation(a, b, p2);

        // If p1 is on the left or collinear, keep it
        if (left1 == 2 || left1 == 0) res.push_back(p1); 
        
        // If the edge crosses the cutting line, find the intersection
        if (left1 != left2 && left1 != 0 && left2 != 0) {
            Point intersect;
            getLineIntersection(p1, p2, a, b, intersect);
            res.push_back(intersect);
        }
    }
    return res;
}

// 11.6 Rotating Calipers (Diameter / Max Distance) O(N)
// ---------------------------------------------------------
// Assumes the polygon is strictly convex and in CCW order.
// Finds the maximum Euclidean distance between any two vertices.
double rotatingCalipers(const vector<Point>& p) {
    int n = p.size();
    if (n == 1) return 0;
    if (n == 2) return dist(p[0], p[1]);
    
    int k = 1;
    // Find the vertex farthest from the baseline p[0]-p[1]
    while (abs(cross(p[1] - p[0], p[(k + 1) % n] - p[0])) < 
           abs(cross(p[1] - p[0], p[(k + 2) % n] - p[0]))) {
        k = (k + 1) % n;
    }
    
    double max_dist = 0;
    int j = k;
    for (int i = 0; i < k || (i == k && j < n); i++) {
        max_dist = max(max_dist, dist(p[i], p[j]));
        // Advance j while the triangle area keeps increasing
        while (j < n && 
               abs(cross(p[(i + 1) % n] - p[i], p[(j + 1) % n] - p[i])) > 
               abs(cross(p[(i + 1) % n] - p[i], p[j] - p[i]))) {
            max_dist = max(max_dist, dist(p[i], p[(j + 1) % n]));
            j++;
        }
    }
    return max_dist;
}

// =========================================================
// 12. Advanced Region Operations (HPI & Minkowski)
// =========================================================

// 12.1 Half-Plane Intersection O(N log N)
// ---------------------------------------------------------
// Represents a directed line. The valid region (half-plane) is strictly 
// to the LEFT of the vector going from 'p' in direction 'v'.
struct DirLine {
    Point p, v;
    double ang;
    DirLine() {}
    DirLine(Point P, Point V) : p(P), v(V) { ang = atan2(v.y, v.x); }
    
    // Sort by angle. If angles are equal, the one further to the "left" is smaller
    bool operator<(const DirLine& u) const {
        if (dcmp(ang, u.ang) == 0) return cross(v, u.p - p) < 0;
        return ang < u.ang;
    }
};

// Check if a point is on the left of the directed line
bool onLeft(DirLine l, Point p) {
    return cross(l.v, p - l.p) > EPS; // > 0 for strict interior, >= 0 to include boundary
}

// Get intersection point of two directed lines
Point getIntersection(DirLine a, DirLine b) {
    Point u = a.p - b.p;
    double t = cross(b.v, u) / cross(a.v, b.v);
    return a.p + a.v * t;
}

// Computes the intersection of multiple half-planes.
// Returns the polygon representing the intersection core.
// NOTE: It is highly recommended to add a large bounding box (4 DirLines) 
// to your input 'lines' to ensure the resulting polygon is closed!
vector<Point> halfPlaneIntersection(vector<DirLine>& lines) {
    int n = lines.size();
    sort(lines.begin(), lines.end());
    
    // Remove lines with identical angles (keep the most restrictive one)
    int first = 0;
    for (int i = 1; i < n; i++) {
        if (dcmp(lines[i].ang, lines[first].ang) != 0) {
            lines[++first] = lines[i];
        }
    }
    n = first + 1;
    lines.resize(n);

    vector<Point> p(n);     // Intersection points
    vector<DirLine> q(n);   // Double-ended queue for lines
    int head = 0, tail = 0;
    q[0] = lines[0];

    for (int i = 1; i < n; i++) {
        // Pop from tail if the intersection of the last two lines is outside the new line
        while (head < tail && !onLeft(lines[i], p[tail - 1])) tail--;
        // Pop from head if the intersection of the first two lines is outside the new line
        while (head < tail && !onLeft(lines[i], p[head])) head++;
        
        q[++tail] = lines[i];
        
        // If two lines in the deque are parallel and in opposite directions, intersection is empty
        if (abs(cross(q[tail].v, q[tail - 1].v)) < EPS) {
            tail--;
            if (!onLeft(q[tail], lines[i].p)) return vector<Point>(); // Empty intersection
        }
        
        if (head < tail) p[tail - 1] = getIntersection(q[tail - 1], q[tail]);
    }

    // Final cleanup: new lines might invalidate the oldest intersections
    while (head < tail && !onLeft(q[head], p[tail - 1])) tail--;
    
    // The intersection of the first and last line closes the polygon
    if (tail - head <= 1) return vector<Point>(); // Degenerate / infinite
    p[tail] = getIntersection(q[tail], q[head]);

    vector<Point> res;
    for (int i = head; i <= tail; i++) res.push_back(p[i]);
    return res;
}


// 12.2 Minkowski Sum of Two Convex Polygons O(N + M)
// ---------------------------------------------------------
// Helper: Reorder a convex polygon so the bottom-left point is index 0.
// This is required to properly align the edges for the Minkowski sum.
void reorderPolygon(vector<Point>& P) {
    int pos = 0;
    for (int i = 1; i < P.size(); i++) {
        if (P[i].y < P[pos].y || (P[i].y == P[pos].y && P[i].x < P[pos].x)) {
            pos = i;
        }
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

// Computes the Minkowski sum (A + B) of two strictly CONVEX polygons.
// Polygons must be given in CCW order.
// Returns a new convex polygon representing the sum.
vector<Point> minkowskiSum(vector<Point> P, vector<Point> Q) {
    // 1. Reorder both polygons to start at the bottom-left vertex
    reorderPolygon(P);
    reorderPolygon(Q);

    // 2. Add wrap-around edges to simplify the while loop logic
    P.push_back(P[0]); P.push_back(P[1]);
    Q.push_back(Q[0]); Q.push_back(Q[1]);

    vector<Point> res;
    int i = 0, j = 0;
    
    // 3. Merge edges based on polar angle (similar to merging sorted arrays)
    while (i < P.size() - 2 || j < Q.size() - 2) {
        // The current vertex is the sum of the active vertices in P and Q
        res.push_back(P[i] + Q[j]);
        
        // Compare the direction of the next edge in P vs next edge in Q
        double c = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
        
        if (c >= 0 && i < P.size() - 2) i++; // Edge in P is "before" or collinear with Q
        if (c <= 0 && j < Q.size() - 2) j++; // Edge in Q is "before" or collinear with P
    }
    
    // The resulting polygon may have collinear points on its edges, 
    // run your convexHull function on 'res' if strict convexity (no collinear points) is required.
    return res;
}


// 12.3 Shift Convex Polygon Inward
// ---------------------------------------------------------
// Shifts all edges of a strictly CCW convex polygon INWARD by distance 'd'.
// (Use a negative 'd' to shift outward).
// Relies on the DirLine and halfPlaneIntersection functions from 12.1.
// Returns the new convex polygon. If 'd' is too large causing the polygon 
// to collapse entirely, it gracefully returns an empty vector.
vector<Point> shiftConvexPolygon(const vector<Point>& poly, double d) {
    int n = poly.size();
    vector<DirLine> shiftedLines;
    
    for (int i = 0; i < n; i++) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        Point v = b - a; // Edge direction vector
        
        // Calculate inward normal (90 degrees CCW rotation of edge vector v)
        Point normal = {-v.y, v.x};
        
        // Normalize the vector and scale by distance d
        double length = len(normal);
        if (length > EPS) {
            normal = normal / length * d;
            
            // Shift point 'a' inward by 'd' and create the new directed line
            // The valid half-plane remains to the strictly LEFT of vector 'v'
            shiftedLines.push_back(DirLine(a + normal, v));
        }
    }

    // Pass the shifted half-planes to your existing O(N log N) HPI algorithm
    return halfPlaneIntersection(shiftedLines);
}
