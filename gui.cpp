#include "gui.h"

#include "utils.h"

Global_state global;

Rectangle rectangle_make(int l, int r, int t, int b) {
    Rectangle rect;
    rect.l = l;
    rect.r = r;
    rect.t = t;
    rect.b = b;
    return rect;
}

bool rectangle_valid(Rectangle r) { return r.l <= r.r && r.t <= r.b; }

Rectangle rectangle_intersection(Rectangle a, Rectangle b) {
    Rectangle r;
    r.l = MAX(a.l, b.l);
    r.r = MIN(a.r, b.r);
    r.t = MAX(a.t, b.t);
    r.b = MIN(a.b, b.b);
    return r;
}

Rectangle rectangle_bounding(Rectangle a, Rectangle b) {
    Rectangle r;
    r.l = MIN(a.l, b.l);
    r.r = MAX(a.r, b.r);
    r.t = MIN(a.t, b.t);
    r.b = MAX(a.b, b.b);
    return r;
}

bool rectangle_equals(Rectangle a, Rectangle b) {
    return a.l == b.l && a.r == b.r && a.t == b.t && a.b == b.b;
}

bool rectangle_contains(Rectangle a, int x, int y) {
    return a.l <= x && x < a.r && a.t <= y && y < a.b;
}

Element* create_element(size_t bytes /* must be at least the size of the header! */,
                        Element* parent, uint32_t flags, Message_handler message_class) {
    Element* element = (Element*)calloc(1, bytes);
    element->flags = flags;
    element->message_class = message_class;

    if (parent) {
        element->window = parent->window;
        element->parent = parent;
        parent->child_count++;
        parent->children =
            (Element**)realloc(parent->children, sizeof(Element*) * parent->child_count);
        parent->children[parent->child_count - 1] = element;
    }

    return element;
}

Big_window* _FindWindow(Window window) {
    for (uintptr_t i = 0; i < global.window_count; i++) {
        if (global.windows[i]->x11_window == window) {
            return global.windows[i];
        }
    }

    return NULL;
}

int _window_message(Element* element, Message message, int di, void* dp) {
    (void)element;
    (void)message;
    (void)di;
    (void)dp;
    return 0;
}

int empty_message_handler(Element* element, Message message, int di, void* dp) {
    (void)element;
    (void)message;
    (void)di;
    (void)dp;
    return 0;
}

int element_message(Element* element, Message message, int di, void* dp) {
    if (element->message_user) {
        int result = element->message_user(element, message, di, dp);
        if (result) {
            return result;
        }
    }
    if (element->message_class) {
        return element->message_class(element, message, di, dp);
    } else {
        return 0;
    }
}

Big_window* create_window(const char* cTitle, int width, int height) {
    Big_window* window = (Big_window*)create_element(sizeof(Big_window), NULL, 0, _window_message);
    window->e.window = window;
    global.window_count++;
    global.windows =
        (Big_window**)realloc(global.windows, sizeof(Big_window*) * global.window_count);
    global.windows[global.window_count - 1] = window;

    XSetWindowAttributes attributes = {};
    window->x11_window =
        XCreateWindow(global.display, DefaultRootWindow(global.display), 0, 0, width, height, 0, 0,
                      InputOutput, CopyFromParent, CWOverrideRedirect, &attributes);
    XStoreName(global.display, window->x11_window, cTitle);
    XSelectInput(global.display, window->x11_window,
                 SubstructureNotifyMask | ExposureMask | PointerMotionMask | ButtonPressMask |
                     ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask |
                     EnterWindowMask | LeaveWindowMask | ButtonMotionMask | KeymapStateMask |
                     FocusChangeMask | PropertyChangeMask);
    XMapRaised(global.display, window->x11_window);
    XSetWMProtocols(global.display, window->x11_window, &global.window_close_id, 1);
    window->image =
        XCreateImage(global.display, global.visual, 24, ZPixmap, 0, NULL, 10, 10, 32, 0);
    return window;
}

int message_loop() {
    while (true) {
        XEvent event;
        XNextEvent(global.display, &event);

        if (event.type == ClientMessage &&
            (Atom)event.xclient.data.l[0] == global.window_close_id) {
            return 0;
        } else if (event.type == ConfigureNotify) {
            Big_window* window = _FindWindow(event.xconfigure.window);
            if (!window) continue;

            if (window->width != event.xconfigure.width ||
                window->height != event.xconfigure.height) {
                window->width = event.xconfigure.width;
                window->height = event.xconfigure.height;
                window->image->width = window->width;
                window->image->height = window->height;
                window->image->bytes_per_line = window->width * 4;
                window->image->data = (char*)window->bits;
            }
        }
    }
}

void initialise() {
    global.display = XOpenDisplay(NULL);
    global.visual = XDefaultVisual(global.display, 0);
    global.window_close_id = XInternAtom(global.display, "WM_DELETE_WINDOW", 0);
}
