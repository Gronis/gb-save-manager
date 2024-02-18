#include "graphics.h"
#include "bitmaps.h"
#include "hardware.h"


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
#define       text_insert_gbc_offset        ((text_fast_offset)                 + (text_fast_length)            / 8)
#define       text_leader_offset            ((text_insert_gbc_offset)           + (text_insert_gbc_length)      / 8)
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

#define       pb_end_offset                 ((text_worker_offset)               + (text_worker_length)          / 8)
#define       pb_0_offset                   ((pb_end_offset)                    + (1))
#define       pb_start_offset               ((pb_0_offset)                      + (1))
#define       pb_1_offset                   ((pb_0_offset)                      + (1))
#define       pb_2_offset                   ((pb_1_offset)                      + (1))
#define       pb_3_offset                   ((pb_2_offset)                      + (1))
#define       pb_4_offset                   ((pb_3_offset)                      + (1))
#define       pb_5_offset                   ((pb_4_offset)                      + (1))
#define       pb_6_offset                   ((pb_5_offset)                      + (1))
#define       pb_7_offset                   ((pb_6_offset)                      + (1))
#define       pb_8_offset                   ((pb_7_offset)                      + (1))

#define       tiles_end                     ((pb_8_offset)                      + (1))

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
    text_insert_gbc_offset,
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

    pb_end_offset,
    pb_0_offset,
    pb_1_offset,
    pb_2_offset,
    pb_3_offset,
    pb_4_offset,
    pb_5_offset,
    pb_6_offset,
    pb_7_offset,
    pb_8_offset,

    tiles_end
};

// This function is overwritten when executing rasterize_progress_bar_tiles,
// which fills some of this function code area with progress bar tiles
void rasterize_all_bitmap_tiles_to_VRAM_call_only_once(void) {
    rasterize_tiles(((range_t*)(&tiles + text_a_tile_index)),               (tile_bitmap_t*)&text_a);
    rasterize_tiles(((range_t*)(&tiles + text_b_tile_index)),               (tile_bitmap_t*)&text_b);
    rasterize_tiles(((range_t*)(&tiles + text_backing_up_tile_index)),      (tile_bitmap_t*)&text_backing_up);
    rasterize_tiles(((range_t*)(&tiles + text_backup_tile_index)),          (tile_bitmap_t*)&text_backup);
    rasterize_tiles(((range_t*)(&tiles + text_cartridge_tile_index)),       (tile_bitmap_t*)&text_cartridge);
    rasterize_tiles(((range_t*)(&tiles + text_connect_gbc_tile_index)),     (tile_bitmap_t*)&text_connect_gbc);
    rasterize_tiles(((range_t*)(&tiles + text_false_tile_index)),           (tile_bitmap_t*)&text_false);
    rasterize_tiles(((range_t*)(&tiles + text_fast_tile_index)),            (tile_bitmap_t*)&text_fast);
    rasterize_tiles(((range_t*)(&tiles + text_insert_gbc_tile_index)),      (tile_bitmap_t*)&text_insert_gbc);
    rasterize_tiles(((range_t*)(&tiles + text_leader_tile_index)),          (tile_bitmap_t*)&text_leader);
    rasterize_tiles(((range_t*)(&tiles + text_link_cable_tile_index)),      (tile_bitmap_t*)&text_link_cable);
    rasterize_tiles(((range_t*)(&tiles + text_mode_tile_index)),            (tile_bitmap_t*)&text_mode);
    rasterize_tiles(((range_t*)(&tiles + text_or_remove_the_tile_index)),   (tile_bitmap_t*)&text_or_remove_the);
    rasterize_tiles(((range_t*)(&tiles + text_press_button_to_tile_index)), (tile_bitmap_t*)&text_press_button_to);
    rasterize_tiles(((range_t*)(&tiles + text_restore_tile_index)),         (tile_bitmap_t*)&text_restore);
    rasterize_tiles(((range_t*)(&tiles + text_role_tile_index)),            (tile_bitmap_t*)&text_role);
    rasterize_tiles(((range_t*)(&tiles + text_rom_tile_index)),             (tile_bitmap_t*)&text_rom);
    rasterize_tiles(((range_t*)(&tiles + text_save_tile_index)),            (tile_bitmap_t*)&text_save);
    rasterize_tiles(((range_t*)(&tiles + text_slow_tile_index)),            (tile_bitmap_t*)&text_slow);
    rasterize_tiles(((range_t*)(&tiles + text_start_tile_index)),           (tile_bitmap_t*)&text_start);
    rasterize_tiles(((range_t*)(&tiles + text_state_tile_index)),           (tile_bitmap_t*)&text_state);
    rasterize_tiles(((range_t*)(&tiles + text_to_change_role_tile_index)),  (tile_bitmap_t*)&text_to_change_role);
    rasterize_tiles(((range_t*)(&tiles + text_true_tile_index)),            (tile_bitmap_t*)&text_true);
    rasterize_tiles(((range_t*)(&tiles + text_waiting_for_tile_index)),     (tile_bitmap_t*)&text_waiting_for);
    rasterize_tiles(((range_t*)(&tiles + text_worker_tile_index)),          (tile_bitmap_t*)&text_worker);
    rasterize_progress_bar_tiles();
}

void rasterize_progress_bar_tiles(void) {
    uint8_t row = 0x80;
    uint8_t pattern = 0x80;
    for (uint8_t i = 0; i < 10; ++i){
        tile_bitmap_t bitmap = { {
            row, pattern, pattern, pattern,
            pattern, pattern, pattern, row,
        }};
        rasterize_tiles(((range_t*)(&tiles + pb_end_tile_index + i)), &bitmap);
        if (i == 0){
            pattern = 0;
        } else {
            pattern = (pattern >> 1) | 0x80;
        }
        row = 0xFF;
    }
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

void set_tiles_row(uint8_t x, uint8_t y, const range_t tiles) {
    uint8_t width = tiles.end - tiles.start;
    set_tiles_row_repeat((x), (y), tiles, width);
}

void set_tiles_row_repeat(uint8_t x, uint8_t y, range_t tiles, uint8_t width) {
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

void render_message(message_list_t* messages) {
    uint8_t len = messages->len;
    messages++;
    for(uint8_t i = 0; i < len; ++i){
        message_t* m = (message_t*)(messages) + i;
        range_t* range = ((range_t*)(&tiles + m->message_tile_index));
        flush_screen();
        set_tiles_row(m->x, m->y, *range);
        flush_screen();
    }
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
    for (uint8_t r = row; r < 8; r+= 2){
        for (uint8_t s = clear_arr[r], e = clear_arr[r + 1]; s != e; ++s) {
            *(_SCRN1 + s + CLEAR_MESSAGE_TILE_INDEX) = 0;
        }
        flush_screen();
    }
}