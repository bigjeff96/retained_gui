#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// headers
#include "gui.h"
#include "utils.h"

// source files
#include "utils.cpp"
#include "gui.cpp"


int main() {
    Rectangle a = rectangle_make(0, 0, 0, 0);
    printf("Hello, world! and %d\n", a.l);
    return 0;
}
