#include "game.h"

#include <stdio.h>
#include <string.h>

#define PLAYER_START_X (TILE_WIDTH / 2)
#define PLAYER_START_Y (TILE_HEIGHT / 2)

static void enter_tile(
    Game *game,
    FloorDirection direction
)
{
    if (!floor_enter_neighbor(&game->floor, direction)) {
        return;
    }

    switch (direction) {
        case FLOOR_NORTH:
            game->player.x = TILE_WIDTH / 2;
            game->player.y = TILE_HEIGHT - 2;
            break;

        case FLOOR_EAST:
            game->player.x = 1;
            game->player.y = TILE_HEIGHT / 2;
            break;

        case FLOOR_SOUTH:
            game->player.x = TILE_WIDTH / 2;
            game->player.y = 1;
            break;

        case FLOOR_WEST:
            game->player.x = TILE_WIDTH - 2;
            game->player.y = TILE_HEIGHT / 2;
            break;

        case FLOOR_DIRECTION_COUNT:
            break;
    }
}

static bool position_is_inside_map(int x, int y)
{
    return x >= 0 &&
           x < TILE_WIDTH &&
           y >= 0 &&
           y < TILE_HEIGHT;
}

static void try_move_player(
    Game *game,
    int delta_x,
    int delta_y
)
{
    int target_x;
    int target_y;
    const FloorTile *tile;
    char target_tile;

    target_x = game->player.x + delta_x;
    target_y = game->player.y + delta_y;

    if (!position_is_inside_map(target_x, target_y)) {
        return;
    }

    tile = floor_current_tile_const(&game->floor);

    if (tile == NULL) {
        return;
    }

    target_tile = tile->map[target_y][target_x];

    if (target_tile == '/') {
        if (target_y == 0) {
            enter_tile(game, FLOOR_NORTH);
            return;
        }

        if (target_x == TILE_WIDTH - 1) {
            enter_tile(game, FLOOR_EAST);
            return;
        }

        if (target_y == TILE_HEIGHT - 1) {
            enter_tile(game, FLOOR_SOUTH);
            return;
        }

        if (target_x == 0) {
            enter_tile(game, FLOOR_WEST);
            return;
        }
    }

    if (!game_position_is_walkable(
            game,
            target_x,
            target_y)) {
        return;
    }

    game->player.x = target_x;
    game->player.y = target_y;
}

bool game_init(Game *game, uint32_t seed)
{
    if (game == NULL) {
        return false;
    }

    memset(game, 0, sizeof(*game));

    game->rank = 1;
    game->running = true;
    game->mode = GAME_MODE_PLAYING;

    if (!floor_generate(&game->floor, seed)) {
        game->running = false;
        return false;
    }

    game->player.x = PLAYER_START_X;
    game->player.y = PLAYER_START_Y;

    return true;
}

void game_handle_input(Game *game, PlatformKey key)
{
    if (game == NULL || !game->running) {
        return;
    }

    /*
     * Quit confirmation blocks normal movement and actions.
     */
    if (game->mode == GAME_MODE_CONFIRM_QUIT) {
        switch (key) {
            case KEY_QUIT:
            case KEY_INTERACT:
                game->running = false;
                break;

            case KEY_ESCAPE:
                game->mode = GAME_MODE_PLAYING;
                break;

            case KEY_NONE:
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_INVENTORY:
            case KEY_CHARACTER:
            case KEY_UNKNOWN:
            default:
                break;
        }

        return;
    }

    /*
     * Normal gameplay input.
     */
    switch (key) {
        case KEY_UP:
            try_move_player(game, 0, -1);
            break;

        case KEY_DOWN:
            try_move_player(game, 0, 1);
            break;

        case KEY_LEFT:
            try_move_player(game, -1, 0);
            break;

        case KEY_RIGHT:
            try_move_player(game, 1, 0);
            break;

        case KEY_QUIT:
        case KEY_ESCAPE:
            game->mode = GAME_MODE_CONFIRM_QUIT;
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
    const FloorTile *tile;
    int x;
    int y;

    if (game == NULL) {
        return;
    }

    tile = floor_current_tile_const(&game->floor);

    if (tile == NULL) {
        return;
    }

    platform_move_cursor(0, 0);

    puts("WRATH BLAST");

    printf(
        "Rank %d | Tile %d of %d\n\n",
        game->rank,
        game->floor.current_tile + 1,
        game->floor.tile_count
    );

    for (y = 0; y < TILE_HEIGHT; ++y) {
        for (x = 0; x < TILE_WIDTH; ++x) {
            if (x == game->player.x &&
                y == game->player.y) {
                putchar('@');
            } else {
                putchar(tile->map[y][x]);
            }
        }

        putchar('\n');
    }

    putchar('\n');

    platform_clear_line();

    if (game->mode == GAME_MODE_CONFIRM_QUIT) {
        puts("ABANDON THIS RUN?");
    } else {
        puts("/: doorway    Movement: WASD or arrows    Quit: Q");
    }

    platform_clear_line();

    if (game->mode == GAME_MODE_CONFIRM_QUIT) {
        puts("Q or Enter: confirm    Escape: cancel");
    } else {
        putchar('\n');
    }

    platform_flush();
}

bool game_position_is_walkable(
    const Game *game,
    int x,
    int y
)
{
    const FloorTile *tile;
    char map_tile;

    if (game == NULL ||
        !position_is_inside_map(x, y)) {
        return false;
    }

    tile = floor_current_tile_const(&game->floor);

    if (tile == NULL) {
        return false;
    }

    map_tile = tile->map[y][x];

    return map_tile == '.' ||
           map_tile == '/' ||
           map_tile == 'x';
}
