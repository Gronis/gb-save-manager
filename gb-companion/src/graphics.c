#include "graphics.h"
#include "bitmaps.h"
#include "hardware.h"

void rasterize_all_bitmap_tiles_to_VRAM(void){
    rasterize_tiles(text_a_tile,               (tile_bitmap_t*)&text_a);
    rasterize_tiles(text_b_tile,               (tile_bitmap_t*)&text_b);
    rasterize_tiles(text_backing_up_tile,      (tile_bitmap_t*)&text_backing_up);
    rasterize_tiles(text_backup_tile,          (tile_bitmap_t*)&text_backup);
    rasterize_tiles(text_cartridge_tile,       (tile_bitmap_t*)&text_cartridge);
    rasterize_tiles(text_connect_gbc_tile,     (tile_bitmap_t*)&text_connect_gbc);
    rasterize_tiles(text_false_tile,           (tile_bitmap_t*)&text_false);
    rasterize_tiles(text_fast_tile,            (tile_bitmap_t*)&text_fast);
    rasterize_tiles(text_leader_tile,          (tile_bitmap_t*)&text_leader);
    rasterize_tiles(text_link_cable_tile,      (tile_bitmap_t*)&text_link_cable);
    rasterize_tiles(text_mode_tile,            (tile_bitmap_t*)&text_mode);
    rasterize_tiles(text_or_remove_the_tile,   (tile_bitmap_t*)&text_or_remove_the);
    rasterize_tiles(text_press_button_to_tile, (tile_bitmap_t*)&text_press_button_to);
    rasterize_tiles(text_restore_tile,         (tile_bitmap_t*)&text_restore);
    rasterize_tiles(text_role_tile,            (tile_bitmap_t*)&text_role);
    rasterize_tiles(text_rom_tile,             (tile_bitmap_t*)&text_rom);
    rasterize_tiles(text_save_tile,            (tile_bitmap_t*)&text_save);
    rasterize_tiles(text_slow_tile,            (tile_bitmap_t*)&text_slow);
    rasterize_tiles(text_start_tile,           (tile_bitmap_t*)&text_start);
    rasterize_tiles(text_state_tile,           (tile_bitmap_t*)&text_state);
    rasterize_tiles(text_to_change_role_tile,  (tile_bitmap_t*)&text_to_change_role);
    rasterize_tiles(text_true_tile,            (tile_bitmap_t*)&text_true);
    rasterize_tiles(text_waiting_for_tile,     (tile_bitmap_t*)&text_waiting_for);
    rasterize_tiles(text_worker_tile,          (tile_bitmap_t*)&text_worker);
}

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


#define       empty_length                  (8)
#define       empty_offset                  (0)
#define       text_a_offset                 ((empty_offset)                     + (empty_length)                / 8)
#define       text_b_offset                 ((text_a_offset)                    + (text_a_length)               / 8)
#define       text_backing_up_offset        ((text_b_offset)                    + (text_b_length)               / 8)
#define       text_backup_offset            ((text_backing_up_offset)           + (text_backing_up_length)      / 8)
#define       text_cartridge_offset         ((text_backup_offset)               + (text_backup_length)          / 8)
#define       text_connect_gbc_offset       ((text_cartridge_offset)            + (text_cartridge_length)       / 8)
#define       text_false_offset             ((text_connect_gbc_offset)          + (text_connect_gbc_length)     / 8)
#define       text_fast_offset              ((text_false_offset)                + (text_false_length)           / 8)
#define       text_leader_offset            ((text_fast_offset)                 + (text_fast_length)            / 8)
#define       text_link_cable_offset        ((text_leader_offset)               + (text_leader_length)          / 8)
#define       text_mode_offset              ((text_link_cable_offset)           + (text_link_cable_length)      / 8)
#define       text_or_remove_the_offset     ((text_mode_offset)                 + (text_mode_length)            / 8)
#define       text_press_button_to_offset   ((text_or_remove_the_offset)        + (text_or_remove_the_length)   / 8)
#define       text_restore_offset           ((text_press_button_to_offset)      + (text_press_button_to_length) / 8)
#define       text_role_offset              ((text_restore_offset)              + (text_restore_length)         / 8)
#define       text_rom_offset               ((text_role_offset)                 + (text_role_length)            / 8)
#define       text_save_offset              ((text_rom_offset)                  + (text_rom_length)             / 8)
#define       text_slow_offset              ((text_save_offset)                 + (text_save_length)            / 8)
#define       text_start_offset             ((text_slow_offset)                 + (text_slow_length)            / 8)
#define       text_state_offset             ((text_start_offset)                + (text_start_length)           / 8)
#define       text_to_change_role_offset    ((text_state_offset)                + (text_state_length)           / 8)
#define       text_true_offset              ((text_to_change_role_offset)       + (text_to_change_role_length)  / 8)
#define       text_waiting_for_offset       ((text_true_offset)                 + (text_true_length)            / 8)
#define       text_worker_offset            ((text_waiting_for_offset)          + (text_waiting_for_length)     / 8)
#define       tiles_end                     ((text_worker_offset)               + (text_worker_length)          / 8)

const uint8_t tiles[] = {
    empty_offset,
    text_a_offset,
    text_b_offset,
    text_backing_up_offset,
    text_backup_offset,
    text_cartridge_offset,
    text_connect_gbc_offset,
    text_false_offset,
    text_fast_offset,
    text_leader_offset,
    text_link_cable_offset,
    text_mode_offset,
    text_or_remove_the_offset,
    text_press_button_to_offset,
    text_restore_offset,
    text_role_offset,
    text_rom_offset,
    text_save_offset,
    text_slow_offset,
    text_start_offset,
    text_state_offset,
    text_to_change_role_offset,
    text_true_offset,
    text_waiting_for_offset,
    text_worker_offset,
    tiles_end
};

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
