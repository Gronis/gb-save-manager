#include "graphics.h"
#include "bitmaps.h"
#include "hardware.h"

#define BITS_PER_PIXEL 2
#define LINES_PER_TILE 8

// This function is overwritten when executing rasterize_progress_bar_tiles,
// which fills some of this function code area with progress bar tiles
void rasterize_all_bitmap_tiles_to_VRAM_call_only_once(void) {
    uint8_t* dst = _VRAM + text_a_offset * LINES_PER_TILE * BITS_PER_PIXEL;
    uint8_t* src = (uint8_t*)&text_a;
    uint8_t* end = src + n_tiles_total * LINES_PER_TILE;
    while (src <= end){
        *(dst) = *(src);
        dst += BITS_PER_PIXEL;
        src += 1;
    }
}

void set_tiles_row(uint8_t x, uint8_t y, range_t* tile_range) {
    x += SCREEN_COORDINATE_TILE_X;
    y += SCREEN_COORDINATE_TILE_Y;
    uint8_t tile_index = tile_range->start;
    uint8_t* dst = _SCRN1 + ((uint16_t)y) * 32 + x;
    while (tile_index < tile_range->end) {
        *(dst++) = tile_index++;
    }
}

void render_message_impl(message_list_t* messages, bool do_flush_screen) {
    uint8_t len = messages->len;
    messages++;
    for(uint8_t i = 0; i < len; ++i){
        message_t* m = (message_t*)(messages) + i;
        range_t* range = ((range_t*)(&tiles + m->message_tile_index));
        if(do_flush_screen){
            flush_screen();
        }
        set_tiles_row(m->x, m->y, range);
        if(do_flush_screen){
            flush_screen();
        }
    }
}

void render_message_no_screen_flush(message_list_t* messages) {
    render_message_impl(messages, false);
}

void render_message(message_list_t* messages) {
    render_message_impl(messages, true);
}

#define CLEAR_MESSAGE_TILE_INDEX get_position_tile_index(1, 4) 
const uint8_t clear_arr[] = {
    (uint8_t)(get_position_tile_index(1,  4) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_position_tile_index(15, 4) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_position_tile_index(1,  5) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_position_tile_index(15, 5) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_position_tile_index(1,  6) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_position_tile_index(15, 6) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_position_tile_index(1,  7) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_position_tile_index(15, 7) - CLEAR_MESSAGE_TILE_INDEX),
};

void clear_message() {
    clear_message_from_row(0);
}

void clear_message_from_row(uint8_t row) {
    for (uint8_t r = row; r < LINES_PER_TILE; r+= 2){
        for (uint8_t s = clear_arr[r], e = clear_arr[r + 1]; s != e; ++s) {
            *(_SCRN1 + s + CLEAR_MESSAGE_TILE_INDEX) = 0;
        }
        flush_screen();
    }
}