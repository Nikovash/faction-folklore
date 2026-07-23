#ifndef WRATH_BLAST_FLOOR_H
#define WRATH_BLAST_FLOOR_H

#include <stdbool.h>
#include <stdint.h>

#define TILE_WIDTH  40
#define TILE_HEIGHT 16

#define FLOOR_MIN_TILES 6
#define FLOOR_MAX_TILES 13

#define FLOOR_GRID_WIDTH  7
#define FLOOR_GRID_HEIGHT 7

#define NO_TILE (-1)

typedef enum {
    FLOOR_NORTH = 0,
    FLOOR_EAST,
    FLOOR_SOUTH,
    FLOOR_WEST,
    FLOOR_DIRECTION_COUNT
} FloorDirection;

typedef struct {
    char map[TILE_HEIGHT][TILE_WIDTH + 1];

    int grid_x;
    int grid_y;

    int neighbors[FLOOR_DIRECTION_COUNT];
    
    bool is_start;
    bool visited;
} FloorTile;

typedef struct {
    FloorTile tiles[FLOOR_MAX_TILES];

    int tile_count;
    int start_tile;
    int current_tile;

    uint32_t random_state;
} Floor;

/*
 * Generate a complete connected floor using the supplied seed.
 */
bool floor_generate(Floor *floor, uint32_t seed);

/*
 * Return the currently active tile.
 */
FloorTile *floor_current_tile(Floor *floor);
const FloorTile *floor_current_tile_const(const Floor *floor);

/*
 * Return the neighboring tile index in a direction, or NO_TILE.
 */
int floor_neighbor(const Floor *floor, int tile_index, FloorDirection direction);

/*
 * Move the active tile through an existing connection.
 */
bool floor_enter_neighbor(Floor *floor, FloorDirection direction);

#endif
