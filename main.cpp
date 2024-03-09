#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// headers
#include "gui.h"
#include "utils.h"

// source files
#include "gui.cpp"
#include "utils.cpp"

Element *element_a, *element_b;

int element_a_message_class(Element* element, Message message, int di, void* dp) {
    (void)element;
    printf("A class: %d, %d, %p\n", message, di, dp);
    return message == MSG_USER + 1;
}

int element_a_message_user(Element* element, Message message, int di, void* dp) {
    (void)element;
    printf("A user: %d, %d, %p\n", message, di, dp);
    return message == MSG_USER + 2;
}

int element_b_message_class(Element* element, Message message, int di, void* dp) {
    (void)element;
    printf("B class: %d, %d, %p\n", message, di, dp);
    return 0;
}

int main() {
    initialise();
    auto* window = create_window("Hello, world", 300, 200);
    element_a = create_element(sizeof(Element), &window->e, 0, element_a_message_class);
    element_a->message_user = element_a_message_user;
    element_b = create_element(sizeof(Element), element_a, 0, element_b_message_class);
    printf("%d\n", element_message(element_a, Message(MSG_USER + 1), 1, NULL));
    printf("%d\n", element_message(element_a, Message(MSG_USER + 2), 2, NULL));
    printf("%d\n", element_message(element_a, Message(MSG_USER + 3), 3, NULL));
    printf("%d\n", element_message(element_b, Message(MSG_USER + 1), 1, NULL));
    printf("%d\n", element_message(element_b, Message(MSG_USER + 2), 2, NULL));
    printf("%d\n", element_message(element_b, Message(MSG_USER + 3), 3, NULL));
    return message_loop();
}
