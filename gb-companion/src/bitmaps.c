
#include "graphics.h"
#include "bitmaps.h"
// Messages are put in every other byte in the same location as bitmaps
// are rendered to. This way, we can store the message and render with
// minimal effort while reusing this "unuseable" memory.

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
    0xFF, text_start_offset,
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
    0xFF, 7,
    0xFF, 2, 0xFF, 4, 0xFF, text_press_button_to_tile_index,
    0xFF, 4, 0xFF, 6, 0xFF, text_a_tile_index,                  0xFF, 5, 0xFF, 6, 0xFF, text_backup_tile_index,     0xFF, 10, 0xFF, 6, 0xFF, text_save_tile_index,
    0xFF, 4, 0xFF, 7, 0xFF, text_b_tile_index,                  0xFF, 5, 0xFF, 7, 0xFF, text_restore_tile_index,    0xFF, 10, 0xFF, 7, 0xFF, text_save_tile_index,
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
    0xFF, 10,
    0xFF, 3, 0xFF, 6, 0xFF,  pb_start_tile_index,
    0xFF, 4, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 5, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 6, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 7, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 8, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 9, 0xFF, 6, 0xFF,  pb_0_tile_index,
    0xFF, 10, 0xFF, 6, 0xFF, pb_0_tile_index,
    0xFF, 11, 0xFF, 6, 0xFF, pb_0_tile_index,
    0xFF, 12, 0xFF, 6, 0xFF, pb_end_tile_index,
};

const uint8_t message_transfer_done_data[] = {
    0xFF, 1,
    0xFF, 12, 0xFF, 4, 0xFF, text_true_tile_index,
};

// This makes sure we include the binary data from bitmaps here
#define INCLUDE_BIN_DATA_DONT_USE_IN_HEADER
#undef  BITMAPS_H
#include "bitmaps.h"
