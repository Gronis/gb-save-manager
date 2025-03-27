#ifndef MESSAGES_H
#define MESSAGES_H

#include "types.h"

#define message_header                  ((message_list_t*)(&message_header_data))
#define message_role_worker             ((message_list_t*)(&message_role_worker_data))
#define message_role_leader             ((message_list_t*)(&message_role_leader_data))
#define message_cartridge_state_ok      ((message_list_t*)(&message_cartridge_state_ok_data))
#define message_cartridge_state_error   ((message_list_t*)(&message_cartridge_state_error_data))
#define message_link_cable_state_ok     ((message_list_t*)(&message_link_cable_state_ok_data))
#define message_link_cable_state_error  ((message_list_t*)(&message_link_cable_state_error_data))
#define message_idle_role_change_info   ((message_list_t*)(&message_idle_role_change_info_data))
#define message_insert_gbc_cartridge    ((message_list_t*)(&message_insert_gbc_cartridge_data))
#define message_insert_gbc_link_cable   ((message_list_t*)(&message_insert_gbc_link_cable_data))
#define message_waiting_for_leader      ((message_list_t*)(&message_waiting_for_leader_data))
#define message_choose_action           ((message_list_t*)(&message_choose_action_data))
#define message_restoring_save          ((message_list_t*)(&message_restoring_save_data))
#define message_backing_up_save         ((message_list_t*)(&message_backing_up_save_data))
#define message_progress_bar            ((message_list_t*)(&message_progress_bar_data))
#define message_transfer_done           ((message_list_t*)(&message_transfer_done_data))
#define message_transfer_error          ((message_list_t*)(&message_transfer_error_data))

extern const uint8_t                    message_header_data[];
extern const uint8_t                    message_role_worker_data[];
extern const uint8_t                    message_role_leader_data[];
extern const uint8_t                    message_cartridge_state_ok_data[];
extern const uint8_t                    message_cartridge_state_error_data[];
extern const uint8_t                    message_link_cable_state_ok_data[];
extern const uint8_t                    message_link_cable_state_error_data[];
extern const uint8_t                    message_idle_role_change_info_data[];
extern const uint8_t                    message_insert_gbc_cartridge_data[];
extern const uint8_t                    message_insert_gbc_link_cable_data[];
extern const uint8_t                    message_waiting_for_leader_data[];
extern const uint8_t                    message_choose_action_data[];
extern const uint8_t                    message_restoring_save_data[];
extern const uint8_t                    message_backing_up_save_data[];
extern const uint8_t                    message_progress_bar_data[];
extern const uint8_t                    message_transfer_done_data[];
extern const uint8_t                    message_transfer_error_data[];

#endif // MESSAGES_H
