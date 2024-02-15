#pragma once
struct Rectangle {
    int l, r, t, b;
};

struct Global_state {
};

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (b) : (a))

Rectangle rectangle_make(int l, int r, int t, int b);
bool rectangle_valid(Rectangle r);
Rectangle rectangle_intersection(Rectangle a, Rectangle b); // can be invalid
Rectangle rectangle_bounding(Rectangle a, Rectangle b);
bool rectangle_equals(Rectangle a, Rectangle b);
bool rectangle_contains(Rectangle a, int x, int y);


