#include "game.h"

#include <stdio.h>
#include <string.h>

/*
 * Fixed prototype dungeon for Milestone 1.
 *
 * Each row must contain exactly GAME_MAP_WIDTH visible characters.
 */
static const char *const prototype_map[GAME_MAP_HEIGHT] = {
    "########################################",
    "#......................................#",
    "#..######..............########........#",
    "#.......#..............#...............#",
    "#.......#....###########...............#",
    "#.......#..............................#",
    "#.......########.......................#",
    "#......................................#",
    "#..............########................#",
    "#..............#.......................#",
    "#....###########.......................#",
    "#......................................#",
    "#.......................########.......#",
    "#......................................#",
    "#.....................................>#",
    "########################################"
};

static void game_try_move_player(Game *game, int delta_x, int delta_y)
{
    int target_x;
    int target_y;

    target_x = game->player.x + delta_x;
    target_y = game->player.y + delta_y;

    if (!game_position_is_walkable(game, target_x, target_y)) {
        return;
    }

    game->player.x = target_x;
    game->player.y = target_y;
}

void game_init(Game *game)
{
    int y;

    if (game == NULL) {
        return;
    }

    memset(game, 0, sizeof(*game));

    for (y = 0; y < GAME_MAP_HEIGHT; ++y) {
        /*
         * Copy exactly one map row and explicitly terminate it.
         */
        memcpy(game->map[y], prototype_map[y], GAME_MAP_WIDTH);
        game->map[y][GAME_MAP_WIDTH] = '\0';
    }

    game->player.x = 2;
    game->player.y = 2;
    game->running = true;
}

void game_handle_input(Game *game, PlatformKey key)
{
    if (game == NULL || !game->running) {
        return;
    }

    switch (key) {
        case KEY_UP:
            game_try_move_player(game, 0, -1);
            break;

        case KEY_DOWN:
            game_try_move_player(game, 0, 1);
            break;

        case KEY_LEFT:
            game_try_move_player(game, -1, 0);
            break;

        case KEY_RIGHT:
            game_try_move_player(game, 1, 0);
            break;

        case KEY_QUIT:
        case KEY_ESCAPE:
            game->running = false;
            break;

        case KEY_NONE:
        case KEY_INTERACT:
        case KEY_INVENTORY:
        case KEY_CHARACTER:
        case KEY_UNKNOWN:
        default:
            break;
    }
}

void game_render(const Game *game)
{
    int x;
    int y;

    if (game == NULL) {
        return;
    }

    platform_move_cursor(0, 0);

    puts("WRATH BLAST");
    puts("Rank I — Foundation");
    putchar('\n');

    for (y = 0; y < GAME_MAP_HEIGHT; ++y) {
        for (x = 0; x < GAME_MAP_WIDTH; ++x) {
            if (x == game->player.x && y == game->player.y) {
                putchar('@');
            } else {
                putchar(game->map[y][x]);
            }
        }

        putchar('\n');
    }

    putchar('\n');
    puts("Move: WASD or Arrow Keys    Quit: Q or Escape");

    platform_flush();
}

bool game_position_is_walkable(const Game *game, int x, int y)
{
    char tile;

    if (game == NULL) {
        return false;
    }

    if (x < 0 || x >= GAME_MAP_WIDTH ||
        y < 0 || y >= GAME_MAP_HEIGHT) {
        return false;
    }

    tile = game->map[y][x];

    return tile == '.' || tile == '>';
}
