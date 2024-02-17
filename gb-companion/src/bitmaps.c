
#include "graphics.h"
// Messages are put in every other byte in the same location as bitmaps
// are rendered to. This way, we can store the message and render with
// minimal effort while reusing this "unuseable" memory.

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

// This makes sure we include the binary data from bitmaps here
#define INCLUDE_BIN_DATA
#include "bitmaps.h"
