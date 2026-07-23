#ifdef _WIN32

#include "platform.h"

#include <stdio.h>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

static HANDLE input_handle;
static HANDLE output_handle;

static DWORD original_input_mode;
static DWORD original_output_mode;

static int terminal_initialized = 0;

int platform_init(void)
{
    DWORD input_mode;
    DWORD output_mode;
    
    setvbuf(stdout, NULL, _IOFBF, 4096);

    input_handle = GetStdHandle(STD_INPUT_HANDLE);
    output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (input_handle == NULL ||
        input_handle == INVALID_HANDLE_VALUE ||
        output_handle == NULL ||
        output_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Wrath Blast requires a Windows console.\n");
        return -1;
    }

    if (!GetConsoleMode(input_handle, &original_input_mode)) {
        fprintf(stderr, "Unable to read Windows input mode.\n");
        return -1;
    }

    if (!GetConsoleMode(output_handle, &original_output_mode)) {
        fprintf(stderr, "Unable to read Windows output mode.\n");
        return -1;
    }

    input_mode = original_input_mode;
    input_mode &= ~(ENABLE_LINE_INPUT |
                    ENABLE_ECHO_INPUT |
                    ENABLE_PROCESSED_INPUT |
                    ENABLE_QUICK_EDIT_MODE);
    input_mode |= ENABLE_EXTENDED_FLAGS;

    output_mode = original_output_mode;
    output_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(input_handle, input_mode)) {
        fprintf(stderr, "Unable to configure Windows input.\n");
        return -1;
    }

    if (!SetConsoleMode(output_handle, output_mode)) {
        SetConsoleMode(input_handle, original_input_mode);
        fprintf(stderr, "Unable to configure Windows output.\n");
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
        SetConsoleMode(input_handle, original_input_mode);
        SetConsoleMode(output_handle, original_output_mode);
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
    INPUT_RECORD record;
    DWORD records_read;

    for (;;) {
        if (!ReadConsoleInput(
                input_handle,
                &record,
                1,
                &records_read)) {
            return KEY_NONE;
        }

        if (record.EventType != KEY_EVENT) {
            continue;
        }

        if (!record.Event.KeyEvent.bKeyDown) {
            continue;
        }

        switch (record.Event.KeyEvent.wVirtualKeyCode) {
            case VK_UP:
                return KEY_UP;

            case VK_DOWN:
                return KEY_DOWN;

            case VK_LEFT:
                return KEY_LEFT;

            case VK_RIGHT:
                return KEY_RIGHT;

            case VK_RETURN:
                return KEY_INTERACT;

            case VK_ESCAPE:
                return KEY_ESCAPE;

            default:
                break;
        }

        switch (record.Event.KeyEvent.uChar.AsciiChar) {
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

            default:
                return KEY_UNKNOWN;
        }
    }
}

#endif
