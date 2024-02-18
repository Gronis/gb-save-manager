#ifndef BITMAPS_H
#define BITMAPS_H

#include "text_a_bin.h"
#include "text_b_bin.h"
#include "text_backing_up_bin.h"
#include "text_backup_bin.h"
#include "text_cartridge_bin.h"
#include "text_connect_gbc_bin.h"
#include "text_false_bin.h"
#include "text_fast_bin.h"
#include "text_insert_gbc_bin.h"
#include "text_leader_bin.h"
#include "text_link_cable_bin.h"
#include "text_mode_bin.h"
#include "text_or_remove_the_bin.h"
#include "text_press_button_to_bin.h"
#include "text_restore_bin.h"
#include "text_role_bin.h"
#include "text_rom_bin.h"
#include "text_save_bin.h"
#include "text_slow_bin.h"
#include "text_start_plus_bin.h"
#include "text_state_bin.h"
#include "text_to_change_role_bin.h"
#include "text_true_bin.h"
#include "text_waiting_for_bin.h"
#include "text_worker_bin.h"

#include "types.h"

#define cartridge_mbc_3_ram                      ((cartridge_mode_t*)(&cartridge_mbc_3_ram_data))

extern const uint8_t cartridge_mbc_3_ram_data[];

#define empty_length                    (8)
#define empty_offset                    (0)
#define text_a_offset                   ((empty_offset)                     + (empty_length)                / 8)
#define text_b_offset                   ((text_a_offset)                    + (text_a_length)               / 8)
#define text_backing_up_offset          ((text_b_offset)                    + (text_b_length)               / 8)
#define text_backup_offset              ((text_backing_up_offset)           + (text_backing_up_length)      / 8)
#define text_cartridge_offset           ((text_backup_offset)               + (text_backup_length)          / 8)
#define text_connect_gbc_offset         ((text_cartridge_offset)            + (text_cartridge_length)       / 8)
#define text_false_offset               ((text_connect_gbc_offset)          + (text_connect_gbc_length)     / 8)
#define text_fast_offset                ((text_false_offset)                + (text_false_length)           / 8)
#define text_insert_gbc_offset          ((text_fast_offset)                 + (text_fast_length)            / 8)
#define text_leader_offset              ((text_insert_gbc_offset)           + (text_insert_gbc_length)      / 8)
#define text_link_cable_offset          ((text_leader_offset)               + (text_leader_length)          / 8)
#define text_mode_offset                ((text_link_cable_offset)           + (text_link_cable_length)      / 8)
#define text_or_remove_the_offset       ((text_mode_offset)                 + (text_mode_length)            / 8)
#define text_press_button_to_offset     ((text_or_remove_the_offset)        + (text_or_remove_the_length)   / 8)
#define text_restore_offset             ((text_press_button_to_offset)      + (text_press_button_to_length) / 8)
#define text_role_offset                ((text_restore_offset)              + (text_restore_length)         / 8)
#define text_rom_offset                 ((text_role_offset)                 + (text_role_length)            / 8)
#define text_save_offset                ((text_rom_offset)                  + (text_rom_length)             / 8)
#define text_slow_offset                ((text_save_offset)                 + (text_save_length)            / 8)
#define text_start_plus_offset          ((text_slow_offset)                 + (text_slow_length)            / 8)
#define text_state_offset               ((text_start_plus_offset)           + (text_start_plus_length)      / 8)
#define text_to_change_role_offset      ((text_state_offset)                + (text_state_length)           / 8)
#define text_true_offset                ((text_to_change_role_offset)       + (text_to_change_role_length)  / 8)
#define text_waiting_for_offset         ((text_true_offset)                 + (text_true_length)            / 8)
#define text_worker_offset              ((text_waiting_for_offset)          + (text_waiting_for_length)     / 8)
#define pb_end_offset                   ((text_worker_offset)               + (text_worker_length)          / 8)
#define pb_0_offset                     ((pb_end_offset)                    + (1))
#define pb_start_offset                 ((pb_0_offset)                      + (1))
#define pb_1_offset                     ((pb_0_offset)                      + (1))
#define pb_2_offset                     ((pb_1_offset)                      + (1))
#define pb_3_offset                     ((pb_2_offset)                      + (1))
#define pb_4_offset                     ((pb_3_offset)                      + (1))
#define pb_5_offset                     ((pb_4_offset)                      + (1))
#define pb_6_offset                     ((pb_5_offset)                      + (1))
#define pb_7_offset                     ((pb_6_offset)                      + (1))
#define pb_8_offset                     ((pb_7_offset)                      + (1))
#define tiles_end                       ((pb_8_offset)                      + (1))



#define empty_tile_index                (0)
#define text_a_tile_index               ((empty_tile_index) + 2)
#define text_b_tile_index               ((text_a_tile_index) + 2)
#define text_backing_up_tile_index      ((text_b_tile_index) + 2)
#define text_backup_tile_index          ((text_backing_up_tile_index) + 2)
#define text_cartridge_tile_index       ((text_backup_tile_index) + 2)
#define text_connect_gbc_tile_index     ((text_cartridge_tile_index) + 2)
#define text_false_tile_index           ((text_connect_gbc_tile_index) + 2)
#define text_fast_tile_index            ((text_false_tile_index) + 2)
#define text_insert_gbc_tile_index      ((text_fast_tile_index) + 2)
#define text_leader_tile_index          ((text_insert_gbc_tile_index) + 2)
#define text_link_cable_tile_index      ((text_leader_tile_index) + 2)
#define text_mode_tile_index            ((text_link_cable_tile_index) + 2)
#define text_or_remove_the_tile_index   ((text_mode_tile_index) + 2)
#define text_press_button_to_tile_index ((text_or_remove_the_tile_index) + 2)
#define text_restore_tile_index         ((text_press_button_to_tile_index) + 2)
#define text_role_tile_index            ((text_restore_tile_index) + 2)
#define text_rom_tile_index             ((text_role_tile_index) + 2)
#define text_save_tile_index            ((text_rom_tile_index) + 2)
#define text_slow_tile_index            ((text_save_tile_index) + 2)
#define text_start_plus_tile_index      ((text_slow_tile_index) + 2)
#define text_state_tile_index           ((text_start_plus_tile_index) + 2)
#define text_to_change_role_tile_index  ((text_state_tile_index) + 2)
#define text_true_tile_index            ((text_to_change_role_tile_index) + 2)
#define text_waiting_for_tile_index     ((text_true_tile_index) + 2)
#define text_worker_tile_index          ((text_waiting_for_tile_index) + 2)

#define pb_end_tile_index               ((text_worker_tile_index) + 2)
#define pb_0_tile_index                 ((pb_end_tile_index) + 2)
#define pb_start_tile_index             ((pb_0_tile_index) + 2)
#define pb_1_tile_index                 ((pb_0_tile_index) + 2)
#define pb_2_tile_index                 ((pb_1_tile_index) + 2)
#define pb_3_tile_index                 ((pb_2_tile_index) + 2)
#define pb_4_tile_index                 ((pb_3_tile_index) + 2)
#define pb_5_tile_index                 ((pb_4_tile_index) + 2)
#define pb_6_tile_index                 ((pb_5_tile_index) + 2)
#define pb_7_tile_index                 ((pb_6_tile_index) + 2)
#define pb_8_tile_index                 ((pb_7_tile_index) + 2)

#define message_header                      ((message_list_t*)(&message_header_data))
#define message_role_worker                 ((message_list_t*)(&message_role_worker_data))
#define message_role_leader                 ((message_list_t*)(&message_role_leader_data))
#define message_cartridge_state_ok          ((message_list_t*)(&message_cartridge_state_ok_data))
#define message_cartridge_state_error       ((message_list_t*)(&message_cartridge_state_error_data))
#define message_link_cable_state_ok         ((message_list_t*)(&message_link_cable_state_ok_data))
#define message_link_cable_state_error      ((message_list_t*)(&message_link_cable_state_error_data))
#define message_idle_role_change_info       ((message_list_t*)(&message_idle_role_change_info_data))
#define message_insert_gbc_cartridge        ((message_list_t*)(&message_insert_gbc_cartridge_data))
#define message_insert_gbc_link_cable       ((message_list_t*)(&message_insert_gbc_link_cable_data))
#define message_waiting_for_leader          ((message_list_t*)(&message_waiting_for_leader_data))
#define message_choose_action               ((message_list_t*)(&message_choose_action_data))
#define message_restore_save                ((message_list_t*)(&message_restore_save_data))
#define message_backing_up_save             ((message_list_t*)(&message_backing_up_save_data))
#define message_progress_bar                ((message_list_t*)(&message_progress_bar_data))
#define message_transfer_done               ((message_list_t*)(&message_transfer_done_data))
#define message_transfer_error              ((message_list_t*)(&message_transfer_error_data))

extern const uint8_t tiles[];

extern const uint8_t message_header_data[];
extern const uint8_t message_role_worker_data[];
extern const uint8_t message_role_leader_data[];
extern const uint8_t message_cartridge_state_ok_data[];
extern const uint8_t message_cartridge_state_error_data[];
extern const uint8_t message_link_cable_state_ok_data[];
extern const uint8_t message_link_cable_state_error_data[];
extern const uint8_t message_idle_role_change_info_data[];
extern const uint8_t message_insert_gbc_cartridge_data[];
extern const uint8_t message_insert_gbc_link_cable_data[];
extern const uint8_t message_waiting_for_leader_data[];
extern const uint8_t message_choose_action_data[];
extern const uint8_t message_restore_save_data[];
extern const uint8_t message_backing_up_save_data[];
extern const uint8_t message_progress_bar_data[];
extern const uint8_t message_transfer_done_data[];
extern const uint8_t message_transfer_error_data[];

extern const uint8_t text_a[];
extern const uint8_t text_b[];
extern const uint8_t text_backing_up[];
extern const uint8_t text_backup[];
extern const uint8_t text_cartridge[];
extern const uint8_t text_connect_gbc[];
extern const uint8_t text_false[];
extern const uint8_t text_fast[];
extern const uint8_t text_insert_gbc[];
extern const uint8_t text_leader[];
extern const uint8_t text_link_cable[];
extern const uint8_t text_mode[];
extern const uint8_t text_or_remove_the[];
extern const uint8_t text_press_button_to[];
extern const uint8_t text_restore[];
extern const uint8_t text_role[];
extern const uint8_t text_rom[];
extern const uint8_t text_save[];
extern const uint8_t text_slow[];
extern const uint8_t text_start_plus[];
extern const uint8_t text_state[];
extern const uint8_t text_to_change_role[];
extern const uint8_t text_true[];
extern const uint8_t text_waiting_for[];
extern const uint8_t text_worker[];

#endif // BITMAPS_H