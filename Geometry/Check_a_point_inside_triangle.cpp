// check point P is strictly inside of triangle a,b,c

struct pt {
    double x, y;
    bool operator == (pt const& t) const {
        return x == t.x && y == t.y;
    }
};

bool pointInTriangle(pt p, pt a, pt b, pt c) {
    pt v0{c.x - a.x, c.y - a.y};
    pt v1{b.x - a.x, b.y - a.y};
    pt v2{p.x - a.x, p.y - a.y};
    double d00 = v0.x*v0.x + v0.y*v0.y;
    double d01 = v0.x*v1.x + v0.y*v1.y;
    double d11 = v1.x*v1.x + v1.y*v1.y;
    double d20 = v2.x*v0.x + v2.y*v0.y;
    double d21 = v2.x*v1.x + v2.y*v1.y;
    double inv = 1.0 / (d00 * d11 - d01 * d01);
    double w1 = (d11 * d20 - d01 * d21) * inv;
    double w2 = (d00 * d21 - d01 * d20) * inv;
    return w1 > 0 && w2 > 0 && w1 + w2 < 1;
}