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

typedef struct {
    uint8_t FILLER1;
    uint8_t x;
    uint8_t FILLER2;
    uint8_t y;
    uint8_t FILLER3;
    uint8_t message_tile_index;
} message_t;

typedef struct {
    uint8_t FILLER1;
    uint8_t len;
} message_list_t;

// Rasterize tiles to VRAM which can later be used by set_tiles_row
void rasterize_tiles(range_t* tile_index, tile_bitmap_t* tile_bitmap);

// Rasterize all tiles in bitmaps.h to VRAM. This is placed in memory so that it
// overwrites the old locations to reuse memory that is no longer needed.
void rasterize_all_bitmap_tiles_to_VRAM(void);

//
void set_tiles_row(uint8_t x, uint8_t y, const range_t tiles);

// Renders tiles to a single row only, repeats if end is reached
void set_tiles_row_repeat(uint8_t x, uint8_t y, const range_t tiles, uint8_t width);

// Render all messages in list. Note: messages should be placed consecutivly in memory
void render_message(message_list_t* messages);

void clear_message(void);

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
#define text_insert_gbc_tile_index      ((text_fast_tile_index) + 1)
#define text_leader_tile_index          ((text_insert_gbc_tile_index) + 1)
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

#define pb_0_tile_index                 ((text_worker_tile_index) + 1)
#define pb_1_tile_index                 ((pb_0_tile_index) + 1)
#define pb_2_tile_index                 ((pb_1_tile_index) + 1)
#define pb_3_tile_index                 ((pb_2_tile_index) + 1)
#define pb_4_tile_index                 ((pb_3_tile_index) + 1)
#define pb_5_tile_index                 ((pb_4_tile_index) + 1)
#define pb_6_tile_index                 ((pb_5_tile_index) + 1)
#define pb_7_tile_index                 ((pb_6_tile_index) + 1)
#define pb_8_tile_index                 ((pb_7_tile_index) + 1)

#endif // (GRAPHICS_text_worker_tile_index) + 1H