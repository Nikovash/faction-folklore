#ifndef WRATH_BLAST_GAME_H
#define WRATH_BLAST_GAME_H

#include <stdbool.h>
#include <stdint.h>

#include "floor.h"
#include "platform.h"

typedef struct {
    int x;
    int y;
} GamePosition;

typedef enum {
    GAME_MODE_PLAYING = 0,
    GAME_MODE_CONFIRM_QUIT
} GameMode;

typedef struct {
    Floor floor;
    GamePosition player;

    int rank;

    bool running;
    GameMode mode;
} Game;

bool game_init(Game *game, uint32_t seed);

void game_handle_input(Game *game, PlatformKey key);
void game_render(const Game *game);

bool game_position_is_walkable(
    const Game *game,
    int x,
    int y
);

#endif
