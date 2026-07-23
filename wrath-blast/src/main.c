#include "game.h"
#include "platform.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void restore_terminal_at_exit(void)
{
    platform_shutdown();
}

int main(void)
{
    Game game;
    uint32_t seed;

    if (platform_init() != 0) {
        return EXIT_FAILURE;
    }

    if (atexit(restore_terminal_at_exit) != 0) {
        platform_shutdown();
        fprintf(stderr, "Failed to register terminal cleanup.\n");
        return EXIT_FAILURE;
    }

    seed = (uint32_t)time(NULL);

    if (!game_init(&game, seed)) {
        fprintf(stderr, "Failed to generate floor.\n");
        return EXIT_FAILURE;
    }

    while (game.running) {
        PlatformKey key;

        game_render(&game);
        key = platform_read_key();
        game_handle_input(&game, key);
    }

    return EXIT_SUCCESS;
}
