#ifndef WRATH_BLAST_PLATFORM_H
#define WRATH_BLAST_PLATFORM_H

/*
 * Platform interface for Wrath Blast.
 *
 * Game code should use these functions instead of calling terminal or
 * operating-system APIs directly.
 */

typedef enum {
    KEY_NONE = 0,

    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,

    KEY_INTERACT,
    KEY_INVENTORY,
    KEY_CHARACTER,

    KEY_QUIT,
    KEY_ESCAPE,

    KEY_UNKNOWN
} PlatformKey;

/*
 * Configure the terminal for game input and rendering.
 *
 * Returns 0 on success and a non-zero value on failure.
 */
int platform_init(void);

/*
 * Restore the terminal to its original state.
 *
 * This function is safe to call after successful initialization.
 */
void platform_shutdown(void);

/*
 * Clear the terminal and move the cursor to the upper-left corner.
 */
void platform_clear_screen(void);

/*
 * Move the terminal cursor to the given zero-based coordinates.
 */
void platform_move_cursor(int x, int y);

/*
 * Hide or show the terminal cursor.
 */
void platform_hide_cursor(void);
void platform_show_cursor(void);

/*
 * Write pending terminal output immediately.
 */
void platform_flush(void);

/*
 * Wait for and return one translated input key.
 *
 * WASD and arrow keys should map to the directional values above.
 */
PlatformKey platform_read_key(void);

#endif
