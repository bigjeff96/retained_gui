Rectangle rectangle_make(int l, int r, int t, int b) {
    Rectangle rect;
    rect.l = l;
    rect.r = r;
    rect.t = t;
    rect.b = b;
    return rect;
}

bool rectangle_valid(Rectangle r) {
    return r.l <= r.r && r.t <= r.b;
}

Rectangle rectangle_intersection(Rectangle a, Rectangle b) {
    Rectangle r;
    r.l = max(a.l, b.l);
    r.r = min(a.r, b.r);
    r.t = max(a.t, b.t);
    r.b = min(a.b, b.b);
    return r;
}

Rectangle rectangle_bounding(Rectangle a, Rectangle b) {
    Rectangle r;
    r.l = min(a.l, b.l);
    r.r = max(a.r, b.r);
    r.t = min(a.t, b.t);
    r.b = max(a.b, b.b);
    return r;
}

bool rectangle_equals(Rectangle a, Rectangle b) {
    return a.l == b.l && a.r == b.r && a.t == b.t && a.b == b.b;
}

bool rectangle_contains(Rectangle a, int x, int y) {
    return a.l <= x && x < a.r && a.t <= y && y < a.b;
}

