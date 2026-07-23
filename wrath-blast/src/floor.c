#include "floor.h"

#include <stddef.h>
#include <string.h>

#define DOOR_X (TILE_WIDTH / 2)
#define DOOR_Y (TILE_HEIGHT / 2)

static uint32_t floor_random(Floor *floor)
{
    uint32_t value = floor->random_state;

    /*
     * Xorshift32: tiny deterministic random-number generator.
     */
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;

    if (value == 0) {
        value = 0xA341316CU;
    }

    floor->random_state = value;
    return value;
}

static int floor_random_range(Floor *floor, int minimum, int maximum)
{
    uint32_t range;

    if (maximum <= minimum) {
        return minimum;
    }

    range = (uint32_t)(maximum - minimum + 1);

    return minimum + (int)(floor_random(floor) % range);
}

static FloorDirection opposite_direction(FloorDirection direction)
{
    switch (direction) {
        case FLOOR_NORTH:
            return FLOOR_SOUTH;

        case FLOOR_EAST:
            return FLOOR_WEST;

        case FLOOR_SOUTH:
            return FLOOR_NORTH;

        case FLOOR_WEST:
        default:
            return FLOOR_EAST;
    }
}

static void direction_offset(
    FloorDirection direction,
    int *delta_x,
    int *delta_y
)
{
    *delta_x = 0;
    *delta_y = 0;

    switch (direction) {
        case FLOOR_NORTH:
            *delta_y = -1;
            break;

        case FLOOR_EAST:
            *delta_x = 1;
            break;

        case FLOOR_SOUTH:
            *delta_y = 1;
            break;

        case FLOOR_WEST:
            *delta_x = -1;
            break;
    }
}

static int find_tile_at(const Floor *floor, int grid_x, int grid_y)
{
    int index;

    for (index = 0; index < floor->tile_count; ++index) {
        if (floor->tiles[index].grid_x == grid_x &&
            floor->tiles[index].grid_y == grid_y) {
            return index;
        }
    }

    return NO_TILE;
}

static void initialize_tile(FloorTile *tile, int grid_x, int grid_y)
{
    int direction;

    memset(tile, 0, sizeof(*tile));

    tile->grid_x = grid_x;
    tile->grid_y = grid_y;

    for (direction = 0; direction < 4; ++direction) {
        tile->neighbors[direction] = NO_TILE;
    }
}

static void connect_tiles(
    Floor *floor,
    int first_index,
    int second_index,
    FloorDirection direction
)
{
    FloorDirection opposite = opposite_direction(direction);

    floor->tiles[first_index].neighbors[direction] = second_index;
    floor->tiles[second_index].neighbors[opposite] = first_index;
}

static bool try_add_tile(Floor *floor)
{
    int attempt;

    for (attempt = 0; attempt < 128; ++attempt) {
        int parent_index;
        int new_index;
        int target_x;
        int target_y;
        int delta_x;
        int delta_y;

        FloorDirection direction;

        parent_index = floor_random_range(
            floor,
            0,
            floor->tile_count - 1
        );

        direction = (FloorDirection)floor_random_range(
            floor,
            FLOOR_NORTH,
            FLOOR_WEST
        );

        direction_offset(direction, &delta_x, &delta_y);

        target_x = floor->tiles[parent_index].grid_x + delta_x;
        target_y = floor->tiles[parent_index].grid_y + delta_y;

        if (target_x < 0 || target_x >= FLOOR_GRID_WIDTH ||
            target_y < 0 || target_y >= FLOOR_GRID_HEIGHT) {
            continue;
        }

        if (find_tile_at(floor, target_x, target_y) != NO_TILE) {
            continue;
        }

        new_index = floor->tile_count;

        initialize_tile(
            &floor->tiles[new_index],
            target_x,
            target_y
        );

        ++floor->tile_count;

        connect_tiles(
            floor,
            parent_index,
            new_index,
            direction
        );

        return true;
    }

    return false;
}

static void add_extra_connections(Floor *floor)
{
    int tile_index;

    for (tile_index = 0; tile_index < floor->tile_count; ++tile_index) {
        FloorDirection direction;

        for (direction = FLOOR_NORTH;
             direction <= FLOOR_WEST;
             direction = (FloorDirection)(direction + 1)) {
            int neighbor_index;
            int target_x;
            int target_y;
            int delta_x;
            int delta_y;

            if (floor->tiles[tile_index].neighbors[direction] != NO_TILE) {
                continue;
            }

            direction_offset(direction, &delta_x, &delta_y);

            target_x = floor->tiles[tile_index].grid_x + delta_x;
            target_y = floor->tiles[tile_index].grid_y + delta_y;

            neighbor_index = find_tile_at(
                floor,
                target_x,
                target_y
            );

            if (neighbor_index == NO_TILE) {
                continue;
            }

            /*
             * Roughly one chance in four to add an extra loop.
             */
            if (floor_random_range(floor, 0, 3) != 0) {
                continue;
            }

            connect_tiles(
                floor,
                tile_index,
                neighbor_index,
                direction
            );
        }
    }
}

static void fill_tile_with_walls(FloorTile *tile)
{
    int x;
    int y;

    for (y = 0; y < TILE_HEIGHT; ++y) {
        for (x = 0; x < TILE_WIDTH; ++x) {
            tile->map[y][x] = '#';
        }

        tile->map[y][TILE_WIDTH] = '\0';
    }
}

static void carve_room_interior(FloorTile *tile)
{
    int x;
    int y;

    for (y = 1; y < TILE_HEIGHT - 1; ++y) {
        for (x = 1; x < TILE_WIDTH - 1; ++x) {
            tile->map[y][x] = '.';
        }
    }
}

static void add_random_obstacles(Floor *floor, FloorTile *tile)
{
    int obstacle_count;
    int obstacle;

    obstacle_count = floor_random_range(floor, 4, 10);

    for (obstacle = 0; obstacle < obstacle_count; ++obstacle) {
        int x;
        int y;
        int length;
        int offset;
        bool horizontal;

        x = floor_random_range(floor, 3, TILE_WIDTH - 8);
        y = floor_random_range(floor, 3, TILE_HEIGHT - 4);
        length = floor_random_range(floor, 2, 7);
        horizontal = floor_random_range(floor, 0, 1) != 0;

        for (offset = 0; offset < length; ++offset) {
            int wall_x = x;
            int wall_y = y;

            if (horizontal) {
                wall_x += offset;
            } else {
                wall_y += offset;
            }

            if (wall_x > 1 && wall_x < TILE_WIDTH - 2 &&
                wall_y > 1 && wall_y < TILE_HEIGHT - 2) {
                tile->map[wall_y][wall_x] = '#';
            }
        }
    }
}

static void carve_horizontal_path(
    FloorTile *tile,
    int start_x,
    int end_x,
    int y
)
{
    int x;
    int minimum = start_x;
    int maximum = end_x;

    if (minimum > maximum) {
        int temporary = minimum;
        minimum = maximum;
        maximum = temporary;
    }

    for (x = minimum; x <= maximum; ++x) {
        tile->map[y][x] = '.';
    }
}

static void carve_vertical_path(
    FloorTile *tile,
    int x,
    int start_y,
    int end_y
)
{
    int y;
    int minimum = start_y;
    int maximum = end_y;

    if (minimum > maximum) {
        int temporary = minimum;
        minimum = maximum;
        maximum = temporary;
    }

    for (y = minimum; y <= maximum; ++y) {
        tile->map[y][x] = '.';
    }
}

static void carve_required_paths(FloorTile *tile)
{
    /*
     * All doors connect to the center. This guarantees that every doorway
     * remains reachable despite random walls.
     */
    carve_horizontal_path(tile, 1, TILE_WIDTH - 2, DOOR_Y);
    carve_vertical_path(tile, DOOR_X, 1, TILE_HEIGHT - 2);
}

static void place_tile_doors(FloorTile *tile)
{
    if (tile->neighbors[FLOOR_NORTH] != NO_TILE) {
        tile->map[0][DOOR_X] = '/';
        tile->map[1][DOOR_X] = '.';
    }

    if (tile->neighbors[FLOOR_EAST] != NO_TILE) {
        tile->map[DOOR_Y][TILE_WIDTH - 1] = '/';
        tile->map[DOOR_Y][TILE_WIDTH - 2] = '.';
    }

    if (tile->neighbors[FLOOR_SOUTH] != NO_TILE) {
        tile->map[TILE_HEIGHT - 1][DOOR_X] = '/';
        tile->map[TILE_HEIGHT - 2][DOOR_X] = '.';
    }

    if (tile->neighbors[FLOOR_WEST] != NO_TILE) {
        tile->map[DOOR_Y][0] = '/';
        tile->map[DOOR_Y][1] = '.';
    }
}

static void generate_tile_map(Floor *floor, FloorTile *tile)
{
    fill_tile_with_walls(tile);
    carve_room_interior(tile);
    add_random_obstacles(floor, tile);
    carve_required_paths(tile);
    place_tile_doors(tile);
}

bool floor_generate(Floor *floor, uint32_t seed)
{
    int target_count;
    int tile_index;

    if (floor == NULL) {
        return false;
    }

    memset(floor, 0, sizeof(*floor));

    floor->random_state = seed;

    if (floor->random_state == 0) {
        floor->random_state = 0xB17B1A57U;
    }

    target_count = floor_random_range(
        floor,
        FLOOR_MIN_TILES,
        FLOOR_MAX_TILES
    );

    initialize_tile(
        &floor->tiles[0],
        FLOOR_GRID_WIDTH / 2,
        FLOOR_GRID_HEIGHT / 2
    );

    floor->tile_count = 1;

    while (floor->tile_count < target_count) {
        if (!try_add_tile(floor)) {
            return false;
        }
    }

    add_extra_connections(floor);

    floor->start_tile = 0;
    floor->current_tile = 0;

    floor->tiles[0].is_start = true;
    floor->tiles[0].visited = true;

    for (tile_index = 0;
         tile_index < floor->tile_count;
         ++tile_index) {
        generate_tile_map(
            floor,
            &floor->tiles[tile_index]
        );
    }

    return true;
}

FloorTile *floor_current_tile(Floor *floor)
{
    if (floor == NULL ||
        floor->current_tile < 0 ||
        floor->current_tile >= floor->tile_count) {
        return NULL;
    }

    return &floor->tiles[floor->current_tile];
}

const FloorTile *floor_current_tile_const(const Floor *floor)
{
    if (floor == NULL ||
        floor->current_tile < 0 ||
        floor->current_tile >= floor->tile_count) {
        return NULL;
    }

    return &floor->tiles[floor->current_tile];
}

int floor_neighbor(
    const Floor *floor,
    int tile_index,
    FloorDirection direction
)
{
    if (floor == NULL ||
        tile_index < 0 ||
        tile_index >= floor->tile_count ||
        direction < FLOOR_NORTH ||
        direction > FLOOR_WEST) {
        return NO_TILE;
    }

    return floor->tiles[tile_index].neighbors[direction];
}

bool floor_enter_neighbor(Floor *floor, FloorDirection direction)
{
    int neighbor_index;

    if (floor == NULL) {
        return false;
    }

    neighbor_index = floor_neighbor(
        floor,
        floor->current_tile,
        direction
    );

    if (neighbor_index == NO_TILE) {
        return false;
    }

    floor->current_tile = neighbor_index;
    floor->tiles[neighbor_index].visited = true;

    return true;
}
