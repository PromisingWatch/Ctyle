#include "src/terminal.c"
#include <stdio.h>

int main(void) {
    raw_on();
    raw_off();
    hide_cursor();
    printf("HEIGHT: %d, g_width: %d", g_height, g_width);
    getchar();
    show_cursor();
    return 0;
}
