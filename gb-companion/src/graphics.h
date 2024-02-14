#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

// Define tile coordinates for viewport
#define SCREEN_COORDINATE_TILE_X ((uint8_t)(32 - 17))
#define SCREEN_COORDINATE_TILE_Y ((uint8_t)(32 - 10))
#define SCREEN_COORDINATE_TILE_WIDTH ((uint8_t)(14))
#define SCREEN_COORDINATE_TILE_HEIGHT ((uint8_t)(8))
// BG has y-axis offsest 1/2 of a tile so 1 tile goes missing for some top+bottom margin

typedef struct {
    uint8_t line[8];
} tile_bitmap_t;

typedef struct {
    uint8_t start;
    uint8_t end;
} range_t;

// Rasterize tiles to VRAM which can later be used by set_tiles_row
void rasterize_tiles(range_t* tile_index, tile_bitmap_t* tile_bitmap);

// Renders tiles to a single row only, repeats if end is reached
void set_tiles_row_repeat(uint8_t x, uint8_t y, const range_t tiles, uint8_t width);

// Renders tiles to a single row only
#define set_tiles_row(x, y, tiles) (set_tiles_row_repeat((x), (y), (tiles), (tiles).end - (tiles).start))

#endif // GRAPHICS_H