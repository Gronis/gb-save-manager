
#include "graphics.h"
#include "data.h"

#include "area_vram1.h"

// Data is put in every other byte in the same location as tile data is
// stored. This way, we can have useable data in between tiles.

const uint8_t cartridge_mbc_3_ram_data[] = {
    // bank_enable_addr                 (address, 8 msb)
    0x00,
    // bank_enable_value      
    0x0A,
    // bank_disable_value      
    0x00,
    // bank_selector_addr               (address, 8 msb)
    0x40,
    // bank_number_value_start          (inclusive)
    0x00,
    // bank_number_value_end            (exclusive)
    0x04,
    // bank_data_addr_start             (inclusive, address, 8 msb)
    0xA0,
    // bank_data_addr_end               (exclusive, address, 8 msb)
    0xC0,
    // bank_enable_advanced_addr
    0x60,
    // bank_enable_advanced_value
    0x01,
};

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
    text_start_plus_offset,
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
    n_tiles_total,
};

const uint8_t message_header_data[] = { 
    8,
    2,  0, text_role_tile_index,
    2,  1, text_cartridge_tile_index,          8, 1, text_state_tile_index,
    2,  2, text_link_cable_tile_index,         8, 2, text_state_tile_index,
    5,  0, text_worker_tile_index,
    12, 1, text_false_tile_index,
    12, 2, text_false_tile_index,
};

const uint8_t message_role_worker_data[] = { 
    1,
    5,  0, text_worker_tile_index,
};

const uint8_t message_role_leader_data[] = { 
    1,
    5,  0, text_leader_tile_index,
};

const uint8_t message_cartridge_state_ok_data[] = { 
    1,
    12, 1, text_true_tile_index,
};

const uint8_t message_cartridge_state_error_data[] = { 
    1,
    12, 1, text_false_tile_index,
};

const uint8_t message_link_cable_state_ok_data[] = { 
    1,
    12, 2, text_true_tile_index,
};

const uint8_t message_link_cable_state_error_data[] = { 
    1,
    12, 2, text_false_tile_index,
};

const uint8_t message_idle_role_change_info_data[] = { 
    5,
    1,  5, text_connect_gbc_tile_index,         8,  5,  text_link_cable_tile_index,
    1,  6, text_or_remove_the_tile_index,       9,  6,  text_cartridge_tile_index,
    1,  7, text_to_change_role_tile_index,
};

const uint8_t message_insert_gbc_cartridge_data[] = { 
    2,
    1,  5,  text_insert_gbc_tile_index,         9,  5,  text_cartridge_tile_index,
};

const uint8_t message_insert_gbc_link_cable_data[] = { 
    2,
    1,  5,  text_connect_gbc_tile_index,        8,  5,  text_link_cable_tile_index,
};

const uint8_t message_waiting_for_leader_data[] = { 
    2,
    2,  5,  text_waiting_for_tile_index,        8,  5,  text_leader_tile_index,
};

const uint8_t message_choose_action_data[] = { 
    8,
    2,  4,  text_press_button_to_tile_index,
    
    4,  6,  text_a_tile_index,
    5,  6,  text_backup_tile_index,
    10, 6,  text_save_tile_index,
    
    1,  7,  text_start_plus_tile_index,
    4,  7,  text_a_tile_index,
    5,  7,  text_restore_tile_index,
    10, 7,  text_save_tile_index,
};

const uint8_t message_restore_save_data[] = {
    2,
    3,  4,  text_restore_tile_index,            9,  4,  text_save_tile_index,
};

const uint8_t message_backing_up_save_data[] = {
    2,
    3,  4,  text_backing_up_tile_index,         9,  4,  text_save_tile_index,
};

const uint8_t message_progress_bar_data[] = {
    9,
    4,  6,  pb_start_tile_index,
    5,  6,  pb_0_tile_index,
    6,  6,  pb_0_tile_index,
    7,  6,  pb_0_tile_index,
    8,  6,  pb_0_tile_index,
    9,  6,  pb_0_tile_index,
    10, 6,  pb_0_tile_index,
    11, 6,  pb_0_tile_index,
    12, 6,  pb_end_tile_index,
};

const uint8_t message_transfer_done_data[] = {
    1,
    12, 4, text_true_tile_index,
};

const uint8_t message_transfer_error_data[] = {
    1,
    12, 4, text_false_tile_index,
};

// This makes sure we include the binary data from data here
#define INCLUDE_BIN_DATA_DONT_USE_IN_HEADER
#undef  BITMAPS_H
#include "data.h"
