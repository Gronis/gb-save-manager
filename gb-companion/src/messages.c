
#include "types.h"
#include "messages.h"
#include "tiles.h"

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
    9,
    2,  4,  text_press_button_to_tile_index,
    
    4,  6,  text_a_tile_index,
    5,  6,  text_back_tile_index,
    8,  6,  text_up_tile_index,
    10, 6,  text_save_tile_index,
    
    1,  7,  text_start_plus_tile_index,
    4,  7,  text_a_tile_index,
    5,  7,  text_restore_tile_index,
    10, 7,  text_save_tile_index,
};

const uint8_t message_restoring_save_data[] = {
    3,
    3,  4,  text_restore_tile_index,
    7,  4,  text_ing_tile_index,
    9,  4,  text_save_tile_index,
};

const uint8_t message_backing_up_save_data[] = {
    4,
    2,  4,  text_back_tile_index,
    5,  4,  text_ing_tile_index,
    7,  4,  text_up_tile_index,
    9,  4,  text_save_tile_index,
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
