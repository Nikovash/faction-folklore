#include "game.h"
#include "platform.h"

#include <stdio.h>
#include <stdlib.h>

static void restore_terminal_at_exit(void)
{
    platform_shutdown();
}

int main(void)
{
    Game game;

    if (platform_init() != 0) {
        return EXIT_FAILURE;
    }

    if (atexit(restore_terminal_at_exit) != 0) {
        platform_shutdown();
        fprintf(stderr, "Failed to register terminal cleanup.\n");
        return EXIT_FAILURE;
    }

    game_init(&game);

    while (game.running) {
        PlatformKey key;

        game_render(&game);
        key = platform_read_key();
        game_handle_input(&game, key);
    }

    return EXIT_SUCCESS;
}
