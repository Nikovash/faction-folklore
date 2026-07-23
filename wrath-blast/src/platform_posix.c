#ifndef _WIN32

#include "platform.h"

#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static struct termios original_terminal;
static int terminal_initialized = 0;

static int apply_game_terminal_mode(int timed)
{
    struct termios settings = original_terminal;

    settings.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
    settings.c_cc[VMIN] = timed ? 0 : 1;
    settings.c_cc[VTIME] = timed ? 1 : 0;

    return tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

static int read_byte(unsigned char *value)
{
    ssize_t result;

    do {
        result = read(STDIN_FILENO, value, 1);
    } while (result < 0 && errno == EINTR);

    return result == 1;
}

int platform_init(void)
{
	setvbuf(stdout, NULL, _IOFBF, 4096);

    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        fprintf(stderr, "Wrath Blast requires an interactive terminal.\n");
        return -1;
    }

    if (tcgetattr(STDIN_FILENO, &original_terminal) != 0) {
        perror("tcgetattr");
        return -1;
    }

    if (apply_game_terminal_mode(0) != 0) {
        perror("tcsetattr");
        return -1;
    }

    terminal_initialized = 1;

    platform_hide_cursor();
    platform_clear_screen();
    platform_flush();

    return 0;
}

void platform_shutdown(void)
{
    if (terminal_initialized) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
        terminal_initialized = 0;
    }

    platform_show_cursor();
    fputs("\x1b[0m\x1b[2J\x1b[H", stdout);
    platform_flush();
}

void platform_clear_screen(void)
{
    fputs("\x1b[2J\x1b[H", stdout);
}

void platform_clear_line(void)
{
    fputs("\x1b[2K\r", stdout);
}

void platform_move_cursor(int x, int y)
{
    if (x < 0) {
        x = 0;
    }

    if (y < 0) {
        y = 0;
    }

    printf("\x1b[%d;%dH", y + 1, x + 1);
}

void platform_hide_cursor(void)
{
    fputs("\x1b[?25l", stdout);
}

void platform_show_cursor(void)
{
    fputs("\x1b[?25h", stdout);
}

void platform_flush(void)
{
    fflush(stdout);
}

PlatformKey platform_read_key(void)
{
    unsigned char key;

    if (!read_byte(&key)) {
        return KEY_NONE;
    }

    switch (key) {
        case 'w':
        case 'W':
            return KEY_UP;

        case 's':
        case 'S':
            return KEY_DOWN;

        case 'a':
        case 'A':
            return KEY_LEFT;

        case 'd':
        case 'D':
            return KEY_RIGHT;

        case 'e':
        case 'E':
        case '\r':
        case '\n':
            return KEY_INTERACT;

        case 'i':
        case 'I':
            return KEY_INVENTORY;

        case 'c':
        case 'C':
            return KEY_CHARACTER;

        case 'q':
        case 'Q':
            return KEY_QUIT;

        case 27:
            break;

        default:
            return KEY_UNKNOWN;
    }

    /*
     * Arrow keys arrive as three-byte escape sequences:
     *
     * ESC [ A    Up
     * ESC [ B    Down
     * ESC [ C    Right
     * ESC [ D    Left
     */
    if (apply_game_terminal_mode(1) != 0) {
        return KEY_ESCAPE;
    }

    {
        unsigned char second;
        unsigned char third;
        PlatformKey result = KEY_ESCAPE;

        if (read_byte(&second) && second == '[' && read_byte(&third)) {
            switch (third) {
                case 'A':
                    result = KEY_UP;
                    break;

                case 'B':
                    result = KEY_DOWN;
                    break;

                case 'C':
                    result = KEY_RIGHT;
                    break;

                case 'D':
                    result = KEY_LEFT;
                    break;

                default:
                    result = KEY_UNKNOWN;
                    break;
            }
        }

        if (apply_game_terminal_mode(0) != 0) {
            return KEY_UNKNOWN;
        }

        return result;
    }
}

#endif
