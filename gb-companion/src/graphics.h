#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

// Define tile coordinates for viewport
#define SCREEN_COORDINATE_TILE_X ((uint8_t)(32 - 17))
#define SCREEN_COORDINATE_TILE_Y ((uint8_t)(32 - 14))
#define SCREEN_COORDINATE_TILE_WIDTH ((uint8_t)(14))
#define SCREEN_COORDINATE_TILE_HEIGHT ((uint8_t)(8))

#define get_position_tile_index(x, y) ((uint16_t)(SCREEN_COORDINATE_TILE_X) + (x) + (uint16_t)(SCREEN_COORDINATE_TILE_Y + (y)) * 32)
// BG has y-axis offsest 1/2 of a tile so 1 tile goes missing for some top+bottom margin

typedef struct {
    uint8_t line[8];
} tile_bitmap_t;

typedef struct {
    uint8_t start;
    uint8_t end;
} range_t;

typedef struct {
    uint8_t len;
} message_list_t;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t message_tile_index;
} message_t;

// Rasterize all tiles in data.h to VRAM. This is placed in memory so that it
// overwrites the old locations to reuse memory that is no longer needed.
void rasterize_all_bitmap_tiles_to_VRAM_call_only_once(void);

//
void set_tiles_row(uint8_t x, uint8_t y, range_t* tile_range);

// Render all messages in list without flushing the screen in between messages.
// Note: messages should be placed consecutivly in memory
void render_message_no_screen_flush(message_list_t* messages);

// Render all messages in list.
// Note: messages should be placed consecutivly in memory
void render_message(message_list_t* messages);

void clear_message(void);
void clear_message_from_row(uint8_t row);

#endif // (GRAPHICS_text_worker_tile_index) + 1H