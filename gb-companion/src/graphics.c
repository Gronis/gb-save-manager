#include "graphics.h"
#include "hardware.h"

void rasterize_tiles(range_t* tile_index, tile_bitmap_t* tile_bitmap) {
    uint16_t _id = ((uint16_t)tile_index->start) * 16;
    uint16_t _end = ((uint16_t)tile_index->end) * 16;
    for (; _id < _end; ++tile_bitmap){
        for (uint8_t i = 0; i < 8; ++i){
            uint8_t line = tile_bitmap->line[i];
            _VRAM[_id] = line;
            // Skip second byte (colors 0/2). Can be any value because of palette
            _id+= 2;
        }
    }
}

void set_tiles_row_repeat(uint8_t x, uint8_t y, const range_t tiles, uint8_t width) {
    x += SCREEN_COORDINATE_TILE_X;
    y += SCREEN_COORDINATE_TILE_Y;
    uint8_t tile_index = tiles.start;
    uint8_t* dst = _SCRN1 + y * 32 + x;
    for (; width; --width) {
        *(dst++) = tile_index++;
        if (tile_index >= tiles.end) {
            tile_index = tiles.start;
        }
    }
}
