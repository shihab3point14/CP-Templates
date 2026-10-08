// ================== CORE POINT STRUCT ==================
struct Point {
    long long x, y;
};

// Common Helper: Cross Product
// Returns > 0 if 'b' is counter-clockwise from 'a'
// Returns < 0 if 'b' is clockwise from 'a'
// Returns 0 if collinear
long long cross_product(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

// Common Helper: Squared Distance (for tie-breaking)
long long distSq(Point p) {
    return p.x * p.x + p.y * p.y;
}

// ================== MODE 1: STANDARD SORT [0, 360) ==================
// Order: Positive X -> Positive Y -> Negative X -> Negative Y
// Usage: Graham Scan, Convex Hull, most geometry problems.

// Returns true if p is in the "Upper" half (0 to 180 degrees)
bool is_upper(Point p) {
    return p.y > 0 || (p.y == 0 && p.x > 0);
}

bool cmpZeroTo360(Point a, Point b) {
    // 1. Sort by Half: Upper half (0-180) comes before Lower half (180-360)
    if (is_upper(a) != is_upper(b)) {
        return is_upper(a);
    }
    
    // 2. Same Half? Use Cross Product
    long long cp = cross_product(a, b);
    if (cp != 0) return cp > 0;
    
    // 3. Collinear? Sort closer points first (Change to > for farther first)
    return distSq(a) < distSq(b);
}

// ================== MODE 2: ATAN2 SORT (-180, 180] ==================
// Order: Negative Y -> Positive X -> Positive Y -> Negative X
// Usage: Problems specifically asking for 'atan2' or 'argument' order.

// Returns true if p is in the "Lower" half (-180 to 0 degrees)
bool is_lower_atan2(Point p) {
    return p.y < 0 || (p.y == 0 && p.x > 0);
}

bool cmpAtan2(Point a, Point b) {
    // 1. Sort by Region: Lower region (-PI, 0] comes before Upper (0, PI]
    if (is_lower_atan2(a) != is_lower_atan2(b)) {
        return is_lower_atan2(a);
    }
    
    // 2. Same Region? Use Cross Product
    long long cp = cross_product(a, b);
    if (cp != 0) return cp > 0;
    
    // 3. Collinear? Sort closer points first
    return distSq(a) < distSq(b);
}