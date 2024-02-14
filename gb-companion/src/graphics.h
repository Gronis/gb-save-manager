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

// Rasterize all tiles in bitmaps.h to VRAM. This is placed in memory so that it
// overwrites the old locations to reuse memory that is no longer needed.
void rasterize_all_bitmap_tiles_to_VRAM(void);

// Renders tiles to a single row only, repeats if end is reached
void set_tiles_row_repeat(uint8_t x, uint8_t y, const range_t tiles, uint8_t width);

// Renders tiles to a single row only
#define set_tiles_row(x, y, tiles) (set_tiles_row_repeat((x), (y), (tiles), (&(tiles))->end - (&(tiles))->start))

extern const uint8_t tiles[];

#define empty_tile                      ((range_t*)(&tiles + 0))
#define text_a_tile                     ((range_t*)(&tiles + 1))
#define text_b_tile                     ((range_t*)(&tiles + 2))
#define text_backing_up_tile            ((range_t*)(&tiles + 3))
#define text_backup_tile                ((range_t*)(&tiles + 4))
#define text_cartridge_tile             ((range_t*)(&tiles + 5))
#define text_connect_gbc_tile           ((range_t*)(&tiles + 6))
#define text_false_tile                 ((range_t*)(&tiles + 7))
#define text_fast_tile                  ((range_t*)(&tiles + 8))
#define text_leader_tile                ((range_t*)(&tiles + 9))
#define text_link_cable_tile            ((range_t*)(&tiles + 10))
#define text_mode_tile                  ((range_t*)(&tiles + 11))
#define text_or_remove_the_tile         ((range_t*)(&tiles + 12))
#define text_press_button_to_tile       ((range_t*)(&tiles + 13))
#define text_restore_tile               ((range_t*)(&tiles + 14))
#define text_role_tile                  ((range_t*)(&tiles + 15))
#define text_rom_tile                   ((range_t*)(&tiles + 16))
#define text_save_tile                  ((range_t*)(&tiles + 17))
#define text_slow_tile                  ((range_t*)(&tiles + 18))
#define text_start_tile                 ((range_t*)(&tiles + 19))
#define text_state_tile                 ((range_t*)(&tiles + 20))
#define text_to_change_role_tile        ((range_t*)(&tiles + 21))
#define text_true_tile                  ((range_t*)(&tiles + 22))
#define text_waiting_for_tile           ((range_t*)(&tiles + 23))
#define text_worker_tile                ((range_t*)(&tiles + 24))

#endif // GRAPHICS_H