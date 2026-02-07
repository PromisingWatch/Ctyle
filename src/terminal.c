#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// GLOBAL VARIABLES

uint16_t g_height;
uint16_t g_width;

// DECLARATIONS

void clear_screen(void);
void raw_off(void);
void raw_on(void);
struct winsize get_term_size(void);

// IMPLEMENTATION

struct termios orig_termios;

void raw_off(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        fprintf(stderr, "tcsetattr");
        exit(2);
    }
}

void raw_on(void) {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        fprintf(stderr, "tcgetattr");
        exit(1);
    }
    atexit(raw_off);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        fprintf(stderr, "tcsetattr");
        exit(2);
    }

    struct winsize sz = {0};
    int status = ioctl(0, TIOCGWINSZ, &sz);
    if (status == -1) {
        perror("ioctl");
        exit(3);
    }

    g_height = sz.ws_row;
    g_width = sz.ws_col;
}

void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void hide_cursor(void) {
    printf("\e[?25l");
    fflush(stdout);
}

void show_cursor(void) {
    printf("\e[?25h");
    return;
}
