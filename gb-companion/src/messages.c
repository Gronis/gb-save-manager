
#include "hardware.h"
#include "types.h"
#include "messages.h"
#include "tiles.h"
#include "graphics.h"

// // TODO: This is a test. remove
// // This will ensure code is put inside RAM
// #include "area_ram.h"

#define pos(x, y)                                       \
    ((uint8_t)(get_tile_position((x),  (y)))),          \
    ((uint8_t)(get_tile_position((x),  (y)) >> 8))

const uint8_t message_header_data[] = {
    8,
    pos(2,  0), text_role_tile_index,
    pos(2,  1), text_cartridge_tile_index,  pos(8, 1), text_state_tile_index,
    pos(2,  2), text_link_cable_tile_index, pos(8, 2), text_state_tile_index,
    pos(5,  0), text_worker_tile_index,
    pos(12, 1), text_false_tile_index,
    pos(12, 2), text_false_tile_index,
};

const uint8_t message_role_worker_data[] = {
    1,
    pos(5,  0), text_worker_tile_index,
};

const uint8_t message_role_leader_data[] = {
    1,
    pos(5,  0), text_leader_tile_index,
};

const uint8_t message_cartridge_state_ok_data[] = {
    1,
    pos(12, 1), text_true_tile_index,
};

const uint8_t message_cartridge_state_error_data[] = {
    1,
    pos(12, 1), text_false_tile_index,
};

const uint8_t message_link_cable_state_ok_data[] = {
    1,
    pos(12, 2), text_true_tile_index,
};

const uint8_t message_link_cable_state_error_data[] = {
    1,
    pos(12, 2), text_false_tile_index,
};

const uint8_t message_idle_role_change_info_data[] = {
    5,
    pos(1,  5), text_connect_gbc_tile_index,    pos(8,  5),  text_link_cable_tile_index,
    pos(1,  6), text_or_remove_the_tile_index,  pos(9,  6),  text_cartridge_tile_index,
    pos(1,  7), text_to_change_role_tile_index,
};

const uint8_t message_insert_gbc_cartridge_data[] = {
    2,
    pos(1,  5),  text_insert_gbc_tile_index,    pos(9,  5),  text_cartridge_tile_index,
};

const uint8_t message_insert_gbc_link_cable_data[] = {
    2,
    pos(1,  5),  text_connect_gbc_tile_index,   pos(8,  5),  text_link_cable_tile_index,
};

const uint8_t message_waiting_for_leader_data[] = {
    2,
    pos(2,  5),  text_waiting_for_tile_index,   pos(8,  5),  text_leader_tile_index,
};

const uint8_t message_choose_action_data[] = {
    9,
    pos(2,  4),  text_press_button_to_tile_index,

    pos(4,  6),  text_a_tile_index,
    pos(5,  6),  text_back_tile_index,
    pos(8,  6),  text_up_tile_index,
    pos(10, 6),  text_save_tile_index,

    pos(1,  7),  text_start_plus_tile_index,
    pos(4,  7),  text_a_tile_index,
    pos(5,  7),  text_restore_tile_index,
    pos(10, 7),  text_save_tile_index,
};

const uint8_t message_restoring_save_data[] = {
    3,
    pos(3,  4),  text_restore_tile_index,
    pos(7,  4),  text_ing_tile_index,
    pos(9,  4),  text_save_tile_index,
};

const uint8_t message_backing_up_save_data[] = {
    4,
    pos(2,  4),  text_back_tile_index,
    pos(5,  4),  text_ing_tile_index,
    pos(7,  4),  text_up_tile_index,
    pos(9,  4),  text_save_tile_index,
};

const uint8_t message_progress_bar_data[] = {
    9,
    pos(4,  6),  pb_start_tile_index,
    pos(5,  6),  pb_0_tile_index,
    pos(6,  6),  pb_0_tile_index,
    pos(7,  6),  pb_0_tile_index,
    pos(8,  6),  pb_0_tile_index,
    pos(9,  6),  pb_0_tile_index,
    pos(10, 6),  pb_0_tile_index,
    pos(11, 6),  pb_0_tile_index,
    pos(12, 6),  pb_end_tile_index,
};

const uint8_t message_transfer_done_data[] = {
    1,
    pos(12, 4), text_true_tile_index,
};

const uint8_t message_transfer_error_data[] = {
    1,
    pos(12, 4), text_false_tile_index,
};

const uint8_t message_resize_save_data[] = {
    3,
    pos(4,  0),  text_resize_tile_index,
    pos(8,  0),  text_save_tile_index,
};

const uint8_t message_8_data[] = {
    2,
    pos(6,  8),  text_8_tile_index,
    pos(7,  8),  text_kb_file_tile_index,
};

const uint8_t message_32_data[] = {
    2,
    pos(5,  8),  text_32_tile_index,
    pos(7,  8),  text_kb_file_tile_index,
};

const uint8_t message_64_data[] = {
    2,
    pos(5,  8),  text_64_tile_index,
    pos(7,  8),  text_kb_file_tile_index,
};

const uint8_t message_128_data[] = {
    3,
    pos(5,  8),  text_12_tile_index,
    pos(6,  8),  text_8_tile_index,
    pos(7,  8),  text_kb_file_tile_index,
};

const uint8_t message_qr_8_kb_data[] = {
    5,
    pos(5,  1),  qr_08_r1_index,
    pos(5,  2),  qr_08_r2_index,
    pos(5,  3),  qr_08_r3_index,
    pos(5,  4),  qr_08_r4_index,
    pos(5,  5),  qr_08_r5_index,
};

const uint8_t message_qr_32_kb_data[] = {
    10,
    pos(5,  1),  qr_08_r1_index,
    pos(5,  2),  qr_08_r2_index,
    pos(5,  3),  qr_08_r3_index,
    pos(5,  4),  qr_08_r4_index,
    pos(5,  5),  qr_08_r5_index,

    pos(6,  1),  qr_32_r1_index,
    pos(5,  2),  qr_32_r2_index,
    pos(5,  3),  qr_32_r3_index,
    pos(5,  4),  qr_32_r4_index,
    pos(6,  5),  qr_32_r5_index,
};

const uint8_t message_qr_64_kb_data[] = {
    10,
    pos(5,  1),  qr_08_r1_index,
    pos(5,  2),  qr_08_r2_index,
    pos(5,  3),  qr_08_r3_index,
    pos(5,  4),  qr_08_r4_index,
    pos(5,  5),  qr_08_r5_index,

    pos(6,  1),  qr_64_r1_index,
    pos(5,  2),  qr_64_r2_index,
    pos(5,  3),  qr_64_r3_index,
    pos(5,  4),  qr_64_r4_index,
    pos(6,  5),  qr_64_r5_index,
};
