#include "types.h"
#include "hardware.h"

#include "definitions.h"
#include "transfer.h"
#include "start.h"

// Tiles only use 1-bit per pixel (no shades of gray)
typedef struct {
    uint8_t line[8];
} tile_bitmap_t;

typedef struct {
    uint8_t start;
    uint8_t end;
} range_t;

#define TILE_BASE_X ((uint8_t)(32 - 17))
#define TILE_BASE_Y ((uint8_t)(32 - 10))
#define SCREEN_TILE_WIDTH ((uint8_t)(14))
#define SCREEN_TILE_HEIGHT ((uint8_t)(8)) 
// BG has offsest 1/2 of a tile so 1 tile goes missing for some top+bottom margin

const uint8_t corners_x[] = {
    0, SCREEN_TILE_WIDTH, SCREEN_TILE_WIDTH, 0
};

const uint8_t corners_y[] = {
    0, 0, SCREEN_TILE_HEIGHT, SCREEN_TILE_HEIGHT
};

#include "text_role_bin.h"

const tile_bitmap_t smiley_bitmap = {
    {// line:
        0x3C,  //   ####
        0x42,  //  #    #
        0xA5,  // # #  # #
        0x81,  // #      #
        0xA5,  // # #  # #
        0x99,  // #  ##  #
        0x42,  //  #    #
        0x3C,  //   ####
    }
};

const range_t tiles[] = {
    { 0, 0 + 1 },
    { 1, 1 + 1 },
    { 0, 0 + 1 },
    { 3, 3 + text_role_length / 8 },
};

#define empty (tiles[0])
#define smiley (tiles[1])
#define TEXT_ROLE (tiles[3])

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

// Renders tiles to a single row only
void set_tiles_row_repeat(uint8_t x, uint8_t y, const range_t tiles, uint8_t width) {
    x += TILE_BASE_X;
    y += TILE_BASE_Y;
    uint8_t tile_index = tiles.start;
    uint8_t* dst = _SCRN1 + y * 32 + x;
    for (; width; --width) {
        *(dst++) = tile_index++;
        if (tile_index >= tiles.end) {
            tile_index = tiles.start;
        }
    }
}

#define set_tiles_row(x, y, tiles) (set_tiles_row_repeat((x), (y), (tiles), (tiles).end - (tiles).start))

#ifdef VRAM_VERSION
#include "ram_code_gbc.h"
void copy_ram_functions_to_ram(void) { 
    uint8_t* src = ram_code;
    volatile uint8_t* dst = (uint8_t*)RAM_LOC;
    uint8_t* end = src + ram_code_length;
    bool success = true;
    // If RAM becomes inaccessable during copy process, stall
    for (; src != end; dst+=success, src+=success){
        uint8_t val = *src;
        *dst = val;
        success = *dst == val;
    }
}
#else
// This is the RAM version, so no copy is necessary
void copy_ram_functions_to_ram(void) {}
#endif 

void main(void) {
    rasterize_tiles(&smiley, &smiley_bitmap);
    rasterize_tiles(&TEXT_ROLE, (tile_bitmap_t*)&text_role);
    {
        uint8_t tile_id = 1;
        bool did_write_to_ram = false;

        set_tiles_row(corners_x[3], corners_y[3], tiles[tile_id]);
        // Infinite loop to keep the program running
        while (1) {
            tile_id = (tile_id + 1) & 3;
            // toggle each corner except 1, flushing the screen in between to avoid flickering
            flush_screen();
            for (uint8_t i = 0; i < 3; ++i){
                set_tiles_row_repeat(corners_x[i], corners_y[i], tiles[tile_id], 3);
                flush_screen();
            }
            if(!did_write_to_ram){
                *((uint8_t*)_RAM+100) = 0x44;
                if (*((uint8_t*)_RAM+100) == 0x44) {
                    did_write_to_ram = true;
                    copy_ram_functions_to_ram();
                    run_in_parallel_to_screen(ram_tile_to_checker);
                }
            }
            // If RAM is unreadable, filcker slow, otherwise filcker fast.
            // RAM is unreadable when GBA is in GBC mode while no GBC cartridge is inserted. 
            if (*((uint8_t*)_RAM+100) != 0x44) {
                // wait for 24 frames
                for (uint8_t i = 0; i < 12; ++i){
                    flush_screen();
                }
            }
        }
    }
}
