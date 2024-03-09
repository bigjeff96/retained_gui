#pragma once

#include <cstdint>
// Linux stuff
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

struct Big_window;  // because of def cycle
struct Element;

enum Message {
    MSG_USER,
};

typedef int (*Message_handler)(Element* element, Message message, int di, void* dp);

struct Element {
    uint32_t flags;  // First 16 bits are specific to the type of element (button, label, etc.). The
                     // higher order 16 bits are common to all elements.
    uint32_t child_count;  // The number of child elements.
    Element* parent;       // The parent element.
    Element** children;    // An array of pointers to the child elements.
    Big_window* window;    // The window at the root of the hierarchy.
    Message_handler message_class,
        message_user;  // all buttons are drawn the same way, but what they do it
                       // different and depend on the user of the lib
    void* cp;          // Context pointer (for the user of the library).
};

struct Rectangle {
    int l, r, t, b;
};

struct Big_window {
    Element e;
    u_int32_t* bits;
    int width, height;
    Window x11_window;
    XImage* image;
};

struct Global_state {
    Big_window** windows;
    size_t window_count;
    Display* display;
    Visual* visual;
    Atom window_close_id;
};

Rectangle rectangle_make(int l, int r, int t, int b);

bool rectangle_valid(Rectangle r);

Rectangle rectangle_intersection(Rectangle a, Rectangle b);  // can be invalid

Rectangle rectangle_bounding(Rectangle a, Rectangle b);

bool rectangle_equals(Rectangle a, Rectangle b);

bool rectangle_contains(Rectangle a, int x, int y);

Element* create_element(size_t bytes /* must be at least the size of the header! */,
                        Element* parent, uint32_t flags, Message_handler message_class);

void initialise();

Big_window* create_window(const char* ctitle, int width, int height);

int message_loop();
