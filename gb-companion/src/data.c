
#include "graphics.h"
#include "data.h"

// Data is put in every other byte in the same location as tile data is
// stored. This way, we can have useable data in between tiles.

const uint8_t cartridge_mbc_3_ram_data[] = {
    // bank_enable_addr                 (address, 8 msb)
    0xFF,   0x00,
    // bank_enable_value      
    0xFF,   0x0A,
    // bank_disable_value      
    0xFF,   0x00,
    // bank_selector_addr               (address, 8 msb)
    0xFF,   0x40,
    // bank_number_value_start          (inclusive)
    0xFF,   0x00,
    // bank_number_value_end            (exclusive)
    0xFF,   0x04,
    // bank_data_addr_start             (inclusive, address, 8 msb)
    0xFF,   0xA0,
    // bank_data_addr_end               (exclusive, address, 8 msb)
    0xFF,   0xC0,
    // bank_enable_advanced_addr
    0xFF,   0x60,
    // bank_enable_advanced_value
    0xFF,   0x01,
};

const uint8_t tiles[] = {
    0xFF, empty_offset,
    0xFF, text_a_offset,
    0xFF, text_b_offset,
    0xFF, text_backing_up_offset,
    0xFF, text_backup_offset,
    0xFF, text_cartridge_offset,
    0xFF, text_connect_gbc_offset,
    0xFF, text_false_offset,
    0xFF, text_fast_offset,
    0xFF, text_insert_gbc_offset,
    0xFF, text_leader_offset,
    0xFF, text_link_cable_offset,
    0xFF, text_mode_offset,
    0xFF, text_or_remove_the_offset,
    0xFF, text_press_button_to_offset,
    0xFF, text_restore_offset,
    0xFF, text_role_offset,
    0xFF, text_rom_offset,
    0xFF, text_save_offset,
    0xFF, text_slow_offset,
    0xFF, text_start_plus_offset,
    0xFF, text_state_offset,
    0xFF, text_to_change_role_offset,
    0xFF, text_true_offset,
    0xFF, text_waiting_for_offset,
    0xFF, text_worker_offset,

    0xFF, pb_end_offset,
    0xFF, pb_0_offset,
    0xFF, pb_1_offset,
    0xFF, pb_2_offset,
    0xFF, pb_3_offset,
    0xFF, pb_4_offset,
    0xFF, pb_5_offset,
    0xFF, pb_6_offset,
    0xFF, pb_7_offset,
    0xFF, pb_8_offset,

    0xFF, tiles_end
};

const uint8_t message_header_data[] = { 
    0xFF, 8,
    0xFF, 2, 0xFF, 0, 0xFF, text_role_tile_index,
    0xFF, 2, 0xFF, 1, 0xFF, text_cartridge_tile_index,          0xFF, 8, 0xFF, 1, 0xFF, text_state_tile_index,
    0xFF, 2, 0xFF, 2, 0xFF, text_link_cable_tile_index,         0xFF, 8, 0xFF, 2, 0xFF, text_state_tile_index,
    0xFF, 5, 0xFF, 0, 0xFF, text_worker_tile_index,
    0xFF, 12, 0xFF, 1, 0xFF, text_false_tile_index,
    0xFF, 12, 0xFF, 2, 0xFF, text_false_tile_index,
};

const uint8_t message_role_worker_data[] = { 
    0xFF, 1,
    0xFF, 5, 0xFF, 0, 0xFF, text_worker_tile_index,
};

const uint8_t message_role_leader_data[] = { 
    0xFF, 1,
    0xFF, 5, 0xFF, 0, 0xFF, text_leader_tile_index,
};

const uint8_t message_cartridge_state_ok_data[] = { 
    0xFF, 1,
    0xFF, 12, 0xFF, 1, 0xFF, text_true_tile_index,
};

const uint8_t message_cartridge_state_error_data[] = { 
    0xFF, 1,
    0xFF, 12, 0xFF, 1, 0xFF, text_false_tile_index,
};

const uint8_t message_link_cable_state_ok_data[] = { 
    0xFF, 1,
    0xFF, 12, 0xFF, 2, 0xFF, text_true_tile_index,
};

const uint8_t message_link_cable_state_error_data[] = { 
    0xFF, 1,
    0xFF, 12, 0xFF, 2, 0xFF, text_false_tile_index,
};

const uint8_t message_idle_role_change_info_data[] = { 
    0xFF, 5,
    0xFF, 1, 0xFF, 5, 0xFF, text_connect_gbc_tile_index,        0xFF, 8, 0xFF, 5, 0xFF, text_link_cable_tile_index,
    0xFF, 1, 0xFF, 6, 0xFF, text_or_remove_the_tile_index,      0xFF, 9, 0xFF, 6, 0xFF, text_cartridge_tile_index,
    0xFF, 1, 0xFF, 7, 0xFF, text_to_change_role_tile_index,
};

const uint8_t message_insert_gbc_cartridge_data[] = { 
    0xFF, 2,
    0xFF, 1, 0xFF, 5, 0xFF, text_insert_gbc_tile_index,         0xFF, 9, 0xFF, 5, 0xFF, text_cartridge_tile_index,
};

const uint8_t message_insert_gbc_link_cable_data[] = { 
    0xFF, 2,
    0xFF, 1, 0xFF, 5, 0xFF, text_connect_gbc_tile_index,        0xFF, 8, 0xFF, 5, 0xFF, text_link_cable_tile_index,
};

const uint8_t message_waiting_for_leader_data[] = { 
    0xFF, 2,
    0xFF, 2, 0xFF, 5, 0xFF, text_waiting_for_tile_index,        0xFF, 8, 0xFF, 5, 0xFF, text_leader_tile_index,
};

const uint8_t message_choose_action_data[] = { 
    0xFF, 8,
    0xFF, 2, 0xFF, 4, 0xFF, text_press_button_to_tile_index,
    
    0xFF, 4, 0xFF, 6, 0xFF, text_a_tile_index,
    0xFF, 5, 0xFF, 6, 0xFF, text_backup_tile_index,
    0xFF, 10, 0xFF, 6, 0xFF, text_save_tile_index,
    
    0xFF, 1, 0xFF, 7, 0xFF, text_start_plus_tile_index,
    0xFF, 4, 0xFF, 7, 0xFF, text_a_tile_index,
    0xFF, 5, 0xFF, 7, 0xFF, text_restore_tile_index,
    0xFF, 10, 0xFF, 7, 0xFF, text_save_tile_index,
};

const uint8_t message_restore_save_data[] = {
    0xFF, 2,
    0xFF, 3, 0xFF, 4, 0xFF,  text_restore_tile_index, 0xFF, 9, 0xFF, 4, 0xFF,  text_save_tile_index,
};

const uint8_t message_backing_up_save_data[] = {
    0xFF, 2,
    0xFF, 3, 0xFF, 4, 0xFF,  text_backing_up_tile_index, 0xFF, 9, 0xFF, 4, 0xFF,  text_save_tile_index,
};

const uint8_t message_progress_bar_data[] = {
    0xFF, 9,
    0xFF, 4, 0xFF, 6, 0xFF,  pb_start_tile_index,
    0xFF, 5, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 6, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 7, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 8, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 9, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 10, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 11, 0xFF, 6, 0xFF, pb_0_tile_index,
    0xFF, 12, 0xFF, 6, 0xFF, pb_end_tile_index,
};

const uint8_t message_transfer_done_data[] = {
    0xFF, 1,
    0xFF, 12, 0xFF, 4, 0xFF, text_true_tile_index,
};

const uint8_t message_transfer_error_data[] = {
    0xFF, 1,
    0xFF, 12, 0xFF, 4, 0xFF, text_false_tile_index,
};

// This makes sure we include the binary data from data here
#define INCLUDE_BIN_DATA_DONT_USE_IN_HEADER
#undef  BITMAPS_H
#include "data.h"
