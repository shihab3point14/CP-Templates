struct Point {
    long long x, y;
};

// Returns:
// > 0 if Left Turn (Counter-Clockwise)
// < 0 if Right Turn (Clockwise)
// = 0 if Collinear
long long ccw(Point A, Point B, Point C) {
    long long val = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
    return val;
}