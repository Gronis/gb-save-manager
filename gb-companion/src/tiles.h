
#ifndef TILES_H
#define TILES_H
#include "types.h"

#include "text_a_bin.h"
#include "text_b_bin.h"
#include "text_back_bin.h"
#include "text_cartridge_bin.h"
#include "text_connect_gbc_bin.h"
#include "text_false_bin.h"
#include "text_fast_bin.h"
#include "text_ing_bin.h"
#include "text_insert_gbc_bin.h"
#include "text_kb_file_bin.h"
#include "text_leader_bin.h"
#include "text_link_cable_bin.h"
#include "text_mode_bin.h"
#include "text_or_remove_the_bin.h"
#include "text_press_button_to_bin.h"
#include "text_resize_bin.h"
#include "text_restore_bin.h"
#include "text_role_bin.h"
#include "text_rom_bin.h"
#include "text_save_bin.h"
#include "text_slow_bin.h"
#include "text_start_plus_bin.h"
#include "text_state_bin.h"
#include "text_to_change_role_bin.h"
#include "text_true_bin.h"
#include "text_up_bin.h"
#include "text_waiting_for_bin.h"
#include "text_worker_bin.h"

#include "tile_pb_end_bin.h"
#include "tile_pb_0_bin.h"
#include "tile_pb_1_bin.h"
#include "tile_pb_2_bin.h"
#include "tile_pb_3_bin.h"
#include "tile_pb_4_bin.h"
#include "tile_pb_5_bin.h"
#include "tile_pb_6_bin.h"
#include "tile_pb_7_bin.h"
#include "tile_pb_8_bin.h"

#include "text_12_bin.h"
#include "text_32_bin.h"
#include "text_64_bin.h"
#include "text_8_bin.h"

#include "qr_08_x1y1_bin.h"
#include "qr_08_x1y2_bin.h"
#include "qr_08_x1y3_bin.h"
#include "qr_08_x1y4_bin.h"
#include "qr_08_x1y5_bin.h"

#include "qr_32_x1y1_bin.h"
#include "qr_32_x1y2_bin.h"
#include "qr_32_x1y3_bin.h"
#include "qr_32_x1y4_bin.h"
#include "qr_32_x1y5_bin.h"

#include "qr_64_x1y1_bin.h"
#include "qr_64_x1y2_bin.h"
#include "qr_64_x1y3_bin.h"
#include "qr_64_x1y4_bin.h"
#include "qr_64_x1y5_bin.h"

#define empty_length                    (8)
#define empty_offset                    (0)
#define text_a_offset                   ((empty_offset)                     + (empty_length)                / 8)
#define text_b_offset                   ((text_a_offset)                    + (text_a_length)               / 8)
#define text_back_offset                ((text_b_offset)                    + (text_b_length)               / 8)
#define text_cartridge_offset           ((text_back_offset)                 + (text_back_length)            / 8)
#define text_connect_gbc_offset         ((text_cartridge_offset)            + (text_cartridge_length)       / 8)
#define text_false_offset               ((text_connect_gbc_offset)          + (text_connect_gbc_length)     / 8)
#define text_fast_offset                ((text_false_offset)                + (text_false_length)           / 8)
#define text_ing_offset                 ((text_fast_offset)                 + (text_fast_length)            / 8)
#define text_insert_gbc_offset          ((text_ing_offset)                  + (text_ing_length)             / 8)
#define text_kb_file_offset             ((text_insert_gbc_offset)           + (text_insert_gbc_length)      / 8)
#define text_leader_offset              ((text_kb_file_offset)              + (text_kb_file_length)         / 8)
#define text_link_cable_offset          ((text_leader_offset)               + (text_leader_length)          / 8)
#define text_mode_offset                ((text_link_cable_offset)           + (text_link_cable_length)      / 8)
#define text_or_remove_the_offset       ((text_mode_offset)                 + (text_mode_length)            / 8)
#define text_press_button_to_offset     ((text_or_remove_the_offset)        + (text_or_remove_the_length)   / 8)
#define text_resize_offset              ((text_press_button_to_offset)      + (text_press_button_to_length) / 8)
#define text_restore_offset             ((text_resize_offset)               + (text_resize_length)          / 8)
#define text_role_offset                ((text_restore_offset)              + (text_restore_length)         / 8)
#define text_rom_offset                 ((text_role_offset)                 + (text_role_length)            / 8)
#define text_save_offset                ((text_rom_offset)                  + (text_rom_length)             / 8)
#define text_slow_offset                ((text_save_offset)                 + (text_save_length)            / 8)
#define text_start_plus_offset          ((text_slow_offset)                 + (text_slow_length)            / 8)
#define text_state_offset               ((text_start_plus_offset)           + (text_start_plus_length)      / 8)
#define text_to_change_role_offset      ((text_state_offset)                + (text_state_length)           / 8)
#define text_true_offset                ((text_to_change_role_offset)       + (text_to_change_role_length)  / 8)
#define text_up_offset                  ((text_true_offset)                 + (text_true_length)            / 8)
#define text_waiting_for_offset         ((text_up_offset)                   + (text_up_length)              / 8)
#define text_worker_offset              ((text_waiting_for_offset)          + (text_waiting_for_length)     / 8)
#define pb_end_offset                   ((text_worker_offset)               + (text_worker_length)          / 8)
#define pb_0_offset                     ((pb_end_offset)                    + (tile_pb_end_length)          / 8)
#define pb_start_offset                 ((pb_0_offset)                      + (tile_pb_0_length)            / 8)
#define pb_1_offset                     ((pb_0_offset)                      + (tile_pb_0_length)            / 8)
#define pb_2_offset                     ((pb_1_offset)                      + (tile_pb_1_length)            / 8)
#define pb_3_offset                     ((pb_2_offset)                      + (tile_pb_2_length)            / 8)
#define pb_4_offset                     ((pb_3_offset)                      + (tile_pb_3_length)            / 8)
#define pb_5_offset                     ((pb_4_offset)                      + (tile_pb_4_length)            / 8)
#define pb_6_offset                     ((pb_5_offset)                      + (tile_pb_5_length)            / 8)
#define pb_7_offset                     ((pb_6_offset)                      + (tile_pb_6_length)            / 8)
#define pb_8_offset                     ((pb_7_offset)                      + (tile_pb_7_length)            / 8)
#define text_12_offset                  ((pb_8_offset)                      + (tile_pb_8_length)            / 8)
#define text_32_offset                  ((text_12_offset)                   + (text_12_length)              / 8)
#define text_64_offset                  ((text_32_offset)                   + (text_32_length)              / 8)
#define text_8_offset                   ((text_64_offset)                   + (text_64_length)              / 8)
#define qr_08_x1y1_offset               ((text_8_offset)                    + (text_8_length)               / 8)
#define qr_08_x1y2_offset               ((qr_08_x1y1_offset)                + (qr_08_x1y1_length)           / 8)
#define qr_08_x1y3_offset               ((qr_08_x1y2_offset)                + (qr_08_x1y2_length)           / 8)
#define qr_08_x1y4_offset               ((qr_08_x1y3_offset)                + (qr_08_x1y3_length)           / 8)
#define qr_08_x1y5_offset               ((qr_08_x1y4_offset)                + (qr_08_x1y4_length)           / 8)
#define qr_32_x1y1_offset               ((qr_08_x1y5_offset)                + (qr_08_x1y5_length)           / 8)
#define qr_32_x1y2_offset               ((qr_32_x1y1_offset)                + (qr_32_x1y1_length)           / 8)
#define qr_32_x1y3_offset               ((qr_32_x1y2_offset)                + (qr_32_x1y2_length)           / 8)
#define qr_32_x1y4_offset               ((qr_32_x1y3_offset)                + (qr_32_x1y3_length)           / 8)
#define qr_32_x1y5_offset               ((qr_32_x1y4_offset)                + (qr_32_x1y4_length)           / 8)
#define qr_64_x1y1_offset               ((qr_32_x1y5_offset)                + (qr_32_x1y5_length)           / 8)
#define qr_64_x1y2_offset               ((qr_64_x1y1_offset)                + (qr_64_x1y1_length)           / 8)
#define qr_64_x1y3_offset               ((qr_64_x1y2_offset)                + (qr_64_x1y2_length)           / 8)
#define qr_64_x1y4_offset               ((qr_64_x1y3_offset)                + (qr_64_x1y3_length)           / 8)
#define qr_64_x1y5_offset               ((qr_64_x1y4_offset)                + (qr_64_x1y4_length)           / 8)
#define n_tiles_total                   ((qr_64_x1y5_offset)                + (qr_64_x1y5_length)           / 8)

#define empty_tile_index                (0)
#define text_a_tile_index               ((empty_tile_index)                 + 1)
#define text_b_tile_index               ((text_a_tile_index)                + 1)
#define text_back_tile_index            ((text_b_tile_index)                + 1)
#define text_cartridge_tile_index       ((text_back_tile_index)             + 1)
#define text_connect_gbc_tile_index     ((text_cartridge_tile_index)        + 1)
#define text_false_tile_index           ((text_connect_gbc_tile_index)      + 1)
#define text_fast_tile_index            ((text_false_tile_index)            + 1)
#define text_ing_tile_index             ((text_fast_tile_index)             + 1)
#define text_insert_gbc_tile_index      ((text_ing_tile_index)              + 1)
#define text_kb_file_tile_index         ((text_insert_gbc_tile_index)       + 1)
#define text_leader_tile_index          ((text_kb_file_tile_index)          + 1)
#define text_link_cable_tile_index      ((text_leader_tile_index)           + 1)
#define text_mode_tile_index            ((text_link_cable_tile_index)       + 1)
#define text_or_remove_the_tile_index   ((text_mode_tile_index)             + 1)
#define text_press_button_to_tile_index ((text_or_remove_the_tile_index)    + 1)
#define text_resize_tile_index          ((text_press_button_to_tile_index)  + 1)
#define text_restore_tile_index         ((text_resize_tile_index)           + 1)
#define text_role_tile_index            ((text_restore_tile_index)          + 1)
#define text_rom_tile_index             ((text_role_tile_index)             + 1)
#define text_save_tile_index            ((text_rom_tile_index)              + 1)
#define text_slow_tile_index            ((text_save_tile_index)             + 1)
#define text_start_plus_tile_index      ((text_slow_tile_index)             + 1)
#define text_state_tile_index           ((text_start_plus_tile_index)       + 1)
#define text_to_change_role_tile_index  ((text_state_tile_index)            + 1)
#define text_true_tile_index            ((text_to_change_role_tile_index)   + 1)
#define text_up_tile_index              ((text_true_tile_index)             + 1)
#define text_waiting_for_tile_index     ((text_up_tile_index)               + 1)
#define text_worker_tile_index          ((text_waiting_for_tile_index)      + 1)
#define pb_end_tile_index               ((text_worker_tile_index)           + 1)
#define pb_0_tile_index                 ((pb_end_tile_index)                + 1)
#define pb_start_tile_index             ((pb_0_tile_index)                  + 1)
#define pb_1_tile_index                 ((pb_0_tile_index)                  + 1)
#define pb_2_tile_index                 ((pb_1_tile_index)                  + 1)
#define pb_3_tile_index                 ((pb_2_tile_index)                  + 1)
#define pb_4_tile_index                 ((pb_3_tile_index)                  + 1)
#define pb_5_tile_index                 ((pb_4_tile_index)                  + 1)
#define pb_6_tile_index                 ((pb_5_tile_index)                  + 1)
#define pb_7_tile_index                 ((pb_6_tile_index)                  + 1)
#define pb_8_tile_index                 ((pb_7_tile_index)                  + 1)
#define text_12_tile_index              ((pb_8_tile_index)                  + 1)
#define text_32_tile_index              ((text_12_tile_index)               + 1)
#define text_64_tile_index              ((text_32_tile_index)               + 1)
#define text_8_tile_index               ((text_64_tile_index)               + 1)
#define qr_08_x1y1_index                ((text_8_tile_index)                + 1)
#define qr_08_x1y2_index                ((qr_08_x1y1_index)                 + 1)
#define qr_08_x1y3_index                ((qr_08_x1y2_index)                 + 1)
#define qr_08_x1y4_index                ((qr_08_x1y3_index)                 + 1)
#define qr_08_x1y5_index                ((qr_08_x1y4_index)                 + 1)
#define qr_32_x1y1_index                ((qr_08_x1y5_index)                + 1)
#define qr_32_x1y2_index                ((qr_32_x1y1_index)                 + 1)
#define qr_32_x1y3_index                ((qr_32_x1y2_index)                 + 1)
#define qr_32_x1y4_index                ((qr_32_x1y3_index)                 + 1)
#define qr_32_x1y5_index                ((qr_32_x1y4_index)                 + 1)
#define qr_64_x1y1_index                ((qr_32_x1y5_index)                + 1)
#define qr_64_x1y2_index                ((qr_64_x1y1_index)                 + 1)
#define qr_64_x1y3_index                ((qr_64_x1y2_index)                 + 1)
#define qr_64_x1y4_index                ((qr_64_x1y3_index)                 + 1)
#define qr_64_x1y5_index                ((qr_64_x1y4_index)                 + 1)

extern const uint8_t                    tiles[];

extern const uint8_t                    garbage[];

#endif // TILES_H
