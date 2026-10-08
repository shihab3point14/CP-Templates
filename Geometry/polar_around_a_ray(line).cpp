// ============================================================================
// Polar Sort Around a Reference Ray (Counter-Clockwise Sweep)
// ============================================================================

struct Point {
    long long x, y;
};

// Global or class-level reference points for the ray: l -> r
Point l, r; 

// Cross product: Returns positive if 'p' is strictly left of ray l->r
long long cross(Point l, Point r, Point p) {
    return (r.x - l.x) * (p.y - l.y) - (r.y - l.y) * (p.x - l.x);
}

// Dot product: Used to check if collinear points are in the same direction
long long dot(Point l, Point r, Point p) {
    return (r.x - l.x) * (p.x - l.x) + (r.y - l.y) * (p.y - l.y);
}

// Squared distance: Used as a tie-breaker for points on the exact same ray
long long dist_sq(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// Assigns a quadrant/region relative to the directional ray l->r
// 0: On the ray (0 deg)
// 1: Left side (1 - 179 deg)
// 2: Opposite ray (180 deg)
// 3: Right side (181 - 359 deg)
int get_region(Point p) {
    long long c = cross(l, r, p);
    if (c > 0) return 1; 
    if (c < 0) return 3; 
    
    // c == 0 implies collinearity. Check direction.
    if (dot(l, r, p) >= 0) return 0; 
    return 2;
}

// The Strict Weak Ordering Comparator for std::sort
bool polar_cmp(Point a, Point b) {
    int regA = get_region(a);
    int regB = get_region(b);
    
    // 1. Sort by geometric region first
    if (regA != regB) {
        return regA < regB;
    }
    
    // 2. Sort by polar angle within the same region
    long long c = cross(l, a, b);
    if (c != 0) {
        return c > 0; // True if 'a' is CCW to 'b'
    }
    
    // 3. Tie-breaker: sort by distance to origin point 'l'
    return dist_sq(l, a) < dist_sq(l, b); 
}

/* Usage Example:
    l = {0, 0}; 
    r = {1, 0}; // Sweeping CCW starting from the positive X-axis
    std::sort(points.begin(), points.end(), polar_cmp);
*/