#include "graphics.h"
#include "version.h"
#include "messages.h"
#include "tiles.h"
#include "hardware.h"
#include "start.h"

#define BITS_PER_PIXEL 2
#define LINES_PER_TILE 8

void copy_tiles_to_vram(void) {
    uint8_t* dst = _VRAM + text_a_offset * LINES_PER_TILE * BITS_PER_PIXEL;
    uint8_t* src = (uint8_t*)&text_a;
    uint8_t* end = src + n_tiles_total * LINES_PER_TILE;
    while (src <= end){
        *(dst) = *(src);
        dst += BITS_PER_PIXEL;
        src += 1;
    }
    // Clear out tile arrangment from row 9 since that is where we show our UI
    for (uint8_t* it = (uint8_t*)(_SCRN1 + 9 * 32); it < (uint8_t*)_RAM; ++it){
        *it = 0;
    }
}

void set_tiles_row(uint8_t* tile_position, range_t* tile_range) {
    uint8_t tile_index = tile_range->start;
    while (tile_index < tile_range->end) {
        *(tile_position++) = tile_index++;
    }
}

void render_message_impl(message_list_t* messages, bool do_flush_screen) {
    uint8_t len = messages->len;
    messages++;
    for(uint8_t i = 0; i < len; ++i){
        message_t* m = (message_t*)(messages) + i;
        range_t* range = ((range_t*)(&tiles[m->message_tile_index]));
        if(do_flush_screen){
            flush_screen();
        }
        set_tiles_row(m->tile_position, range);
        if(do_flush_screen){
            flush_screen();
        }
    }
}

void render_message_no_screen_flush(message_list_t* messages) {
    #ifdef RAM_VERSION
    disable_screen();
    #endif // RAM_VERSION
    render_message_impl(messages, false);
    #ifdef RAM_VERSION
    enable_screen();
    #endif // RAM_VERSION
}

void render_message(message_list_t* messages) {
    render_message_impl(messages, true);
}

void clear_tile_from_row_for_area(uint8_t row, const uint8_t *area, uint16_t area_start, uint8_t n_lines) {
    for (uint8_t r = row; r < (n_lines << 1); r+= 2){
        for (uint8_t s = area[r], e = area[r + 1]; s != e; ++s) {
            *as_u8_ptr(s + area_start) = 0;
        }
        flush_screen();
    }
}

#define CLEAR_HEADER_TILE_INDEX get_tile_position(2, 0)
const uint8_t clear_msg_header_area[] = {
    (uint8_t)(get_tile_position(2,  0) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(13, 0) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(2,  1) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(13, 1) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(2,  2) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(13, 2) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(2,  3) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(13, 3) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(2,  4) - CLEAR_HEADER_TILE_INDEX),
    (uint8_t)(get_tile_position(13, 4) - CLEAR_HEADER_TILE_INDEX),
};
void clear_message_header() {
    clear_tile_from_row_for_area(0, clear_msg_header_area, (uint16_t)CLEAR_HEADER_TILE_INDEX, 5);
}

#define CLEAR_MESSAGE_TILE_INDEX get_tile_position(1, 4)
const uint8_t clear_msg_area[] = {
    (uint8_t)(get_tile_position(1,  4) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_tile_position(15, 4) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_tile_position(1,  5) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_tile_position(15, 5) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_tile_position(1,  6) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_tile_position(15, 6) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_tile_position(1,  7) - CLEAR_MESSAGE_TILE_INDEX),
    (uint8_t)(get_tile_position(15, 7) - CLEAR_MESSAGE_TILE_INDEX),
};
void clear_message_from_row(uint8_t row) {
    clear_tile_from_row_for_area(row, clear_msg_area, (uint16_t)CLEAR_MESSAGE_TILE_INDEX, 4);
}

void clear_message() {
    clear_message_from_row(0);
}
