#ifndef WRATH_BLAST_GAME_H
#define WRATH_BLAST_GAME_H

#include <stdbool.h>

#include "platform.h"

/*
 * Milestone 1 uses a fixed-size map.
 *
 * The map is intentionally small enough to display comfortably in a normal
 * terminal while leaving room below it for status and control information.
 */
#define GAME_MAP_WIDTH  40
#define GAME_MAP_HEIGHT 16

typedef struct {
    int x;
    int y;
} GamePosition;

typedef struct {
    char map[GAME_MAP_HEIGHT][GAME_MAP_WIDTH + 1];

    GamePosition player;

    bool running;
} Game;

/*
 * Initialize a new game and load the prototype dungeon.
 */
void game_init(Game *game);

/*
 * Process one translated keyboard input.
 */
void game_handle_input(Game *game, PlatformKey key);

/*
 * Draw the current game state to the terminal.
 */
void game_render(const Game *game);

/*
 * Return true when the given map location can be entered.
 */
bool game_position_is_walkable(const Game *game, int x, int y);

#endif
