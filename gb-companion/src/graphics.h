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
#define set_tiles_row(x, y, tiles) do { \
    range_t* ___range___ = &(tiles); \
    uint8_t ___width___ = ___range___->end - ___range___->start; \
    set_tiles_row_repeat((x), (y), *___range___, ___width___); \
} while(0)

extern const uint8_t tiles[];

#define empty_tile_index                (0)
#define text_a_tile_index               ((empty_tile_index) + 1)
#define text_b_tile_index               ((text_a_tile_index) + 1)
#define text_backing_up_tile_index      ((text_b_tile_index) + 1)
#define text_backup_tile_index          ((text_backing_up_tile_index) + 1)
#define text_cartridge_tile_index       ((text_backup_tile_index) + 1)
#define text_connect_gbc_tile_index     ((text_cartridge_tile_index) + 1)
#define text_false_tile_index           ((text_connect_gbc_tile_index) + 1)
#define text_fast_tile_index            ((text_false_tile_index) + 1)
#define text_leader_tile_index          ((text_fast_tile_index) + 1)
#define text_link_cable_tile_index      ((text_leader_tile_index) + 1)
#define text_mode_tile_index            ((text_link_cable_tile_index) + 1)
#define text_or_remove_the_tile_index   ((text_mode_tile_index) + 1)
#define text_press_button_to_tile_index ((text_or_remove_the_tile_index) + 1)
#define text_restore_tile_index         ((text_press_button_to_tile_index) + 1)
#define text_role_tile_index            ((text_restore_tile_index) + 1)
#define text_rom_tile_index             ((text_role_tile_index) + 1)
#define text_save_tile_index            ((text_rom_tile_index) + 1)
#define text_slow_tile_index            ((text_save_tile_index) + 1)
#define text_start_tile_index           ((text_slow_tile_index) + 1)
#define text_state_tile_index           ((text_start_tile_index) + 1)
#define text_to_change_role_tile_index  ((text_state_tile_index) + 1)
#define text_true_tile_index            ((text_to_change_role_tile_index) + 1)
#define text_waiting_for_tile_index     ((text_true_tile_index) + 1)
#define text_worker_tile_index          ((text_waiting_for_tile_index) + 1)

#define pb_1_tile_index                 ((text_worker_tile_index) + 1)
#define pb_2_tile_index                 ((pb_1_tile_index) + 1)
#define pb_3_tile_index                 ((pb_2_tile_index) + 1)
#define pb_4_tile_index                 ((pb_3_tile_index) + 1)
#define pb_5_tile_index                 ((pb_4_tile_index) + 1)
#define pb_6_tile_index                 ((pb_5_tile_index) + 1)
#define pb_7_tile_index                 ((pb_6_tile_index) + 1)
#define pb_8_tile_index                 ((pb_7_tile_index) + 1)

#define empty_tile                      ((range_t*)(&tiles + empty_tile_index))
#define text_a_tile                     ((range_t*)(&tiles + text_a_tile_index))
#define text_b_tile                     ((range_t*)(&tiles + text_b_tile_index))
#define text_backing_up_tile            ((range_t*)(&tiles + text_backing_up_tile_index))
#define text_backup_tile                ((range_t*)(&tiles + text_backup_tile_index))
#define text_cartridge_tile             ((range_t*)(&tiles + text_cartridge_tile_index))
#define text_connect_gbc_tile           ((range_t*)(&tiles + text_connect_gbc_tile_index))
#define text_false_tile                 ((range_t*)(&tiles + text_false_tile_index))
#define text_fast_tile                  ((range_t*)(&tiles + text_fast_tile_index))
#define text_leader_tile                ((range_t*)(&tiles + text_leader_tile_index))
#define text_link_cable_tile            ((range_t*)(&tiles + text_link_cable_tile_index))
#define text_mode_tile                  ((range_t*)(&tiles + text_mode_tile_index))
#define text_or_remove_the_tile         ((range_t*)(&tiles + text_or_remove_the_tile_index))
#define text_press_button_to_tile       ((range_t*)(&tiles + text_press_button_to_tile_index))
#define text_restore_tile               ((range_t*)(&tiles + text_restore_tile_index))
#define text_role_tile                  ((range_t*)(&tiles + text_role_tile_index))
#define text_rom_tile                   ((range_t*)(&tiles + text_rom_tile_index))
#define text_save_tile                  ((range_t*)(&tiles + text_save_tile_index))
#define text_slow_tile                  ((range_t*)(&tiles + text_slow_tile_index))
#define text_start_tile                 ((range_t*)(&tiles + text_start_tile_index))
#define text_state_tile                 ((range_t*)(&tiles + text_state_tile_index))
#define text_to_change_role_tile        ((range_t*)(&tiles + text_to_change_role_tile_index))
#define text_true_tile                  ((range_t*)(&tiles + text_true_tile_index))
#define text_waiting_for_tile           ((range_t*)(&tiles + text_waiting_for_tile_index))
#define text_worker_tile                ((range_t*)(&tiles + text_worker_tile_index))

#define pb_1_tile                       ((range_t*)(&tiles + pb_1_tile_index))
#define pb_2_tile                       ((range_t*)(&tiles + pb_2_tile_index))
#define pb_3_tile                       ((range_t*)(&tiles + pb_3_tile_index))
#define pb_4_tile                       ((range_t*)(&tiles + pb_4_tile_index))
#define pb_5_tile                       ((range_t*)(&tiles + pb_5_tile_index))
#define pb_6_tile                       ((range_t*)(&tiles + pb_6_tile_index))
#define pb_7_tile                       ((range_t*)(&tiles + pb_7_tile_index))
#define pb_8_tile                       ((range_t*)(&tiles + pb_8_tile_index))

#endif // (GRAPHICS_text_worker_tile_index) + 1H