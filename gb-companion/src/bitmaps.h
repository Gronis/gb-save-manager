#ifndef BITMAPS_H
#define BITMAPS_H

#include "types.h"

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
#include "text_start_bin.h"
#include "text_state_bin.h"
#include "text_to_change_role_bin.h"
#include "text_true_bin.h"
#include "text_waiting_for_bin.h"
#include "text_worker_bin.h"

#define message_header                      ((message_list_t*)(&message_header_data))
#define message_role_worker                 ((message_list_t*)(&message_role_worker_data))
#define message_role_leader                 ((message_list_t*)(&message_role_leader_data))
#define message_cartridge_state_ok          ((message_list_t*)(&message_cartridge_state_ok_data))
#define message_cartridge_state_error       ((message_list_t*)(&message_cartridge_state_error_data))
#define message_link_cable_state_ok         ((message_list_t*)(&message_link_cable_state_ok_data))
#define message_link_cable_state_error      ((message_list_t*)(&message_link_cable_state_error_data))
#define message_idle_role_change_info       ((message_list_t*)(&message_idle_role_change_info_data))
#define message_insert_gbc_cartridge        ((message_list_t*)(&message_insert_gbc_cartridge_data))
#define message_waiting_for_leader          ((message_list_t*)(&message_waiting_for_leader_data))
#define message_choose_action               ((message_list_t*)(&message_choose_action_data))

extern const uint8_t message_header_data[];
extern const uint8_t message_role_worker_data[];
extern const uint8_t message_role_leader_data[];
extern const uint8_t message_cartridge_state_ok_data[];
extern const uint8_t message_cartridge_state_error_data[];
extern const uint8_t message_link_cable_state_ok_data[];
extern const uint8_t message_link_cable_state_error_data[];
extern const uint8_t message_idle_role_change_info_data[];
extern const uint8_t message_insert_gbc_cartridge_data[];
extern const uint8_t message_waiting_for_leader_data[];
extern const uint8_t message_choose_action_data[];

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
extern const uint8_t text_start[];
extern const uint8_t text_state[];
extern const uint8_t text_to_change_role[];
extern const uint8_t text_true[];
extern const uint8_t text_waiting_for[];
extern const uint8_t text_worker[];

#endif // BITMAPS_H