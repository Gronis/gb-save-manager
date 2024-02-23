#include "types.h"
#include "hardware.h"

#include "version.h"
#include "transfer.h"
#include "start.h"
#include "graphics.h"
#include "messages.h"
#include "cartridges.h"
#include "input.h"

#define CARTRIDGE_TITLE ((char*)(0x0134))
const char LEADER_CARTRIDGE_TITLE[] = "GBSAVEMANA";

#define CARTRIDGE_LOGO ((uint8_t*)(0x0104))
const uint8_t CORPORATE_LOGO[] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73
};

#ifdef VRAM_VERSION
extern const uint8_t ram_code[];
#include "ram_code_gbc.h"

void copy_ram_functions_to_ram(void) {
    // Copy the code from the original location in VRAM0 (0x8000-0x9000)
    uint8_t* src = (uint8_t*)(&ram_code - CODE_LOC + (uint16_t)_VRAM);
    uint8_t* end = src + ram_code_length;
    uint8_t* dst = (uint8_t*)_RAM;
    bool success = true;
    // If RAM becomes inaccessable during copy process, stall
    for (; src != end; dst+=success, src+=success){
        uint8_t val = *src;
        *dst = val;
        success = *dst == val;
    }
}
#else
// This is the RAM version, so no copy is necessary
void copy_ram_functions_to_ram(void) {}
#endif // VRAM_VERSION

bool send_detect_link_cable_packet(bool use_internal_clock) {
    uint8_t serial_data = *rSB;
    bool connected = false;
    if ((serial_data == LINK_CABLE_MAGIC_BYTE_SYNC) || 
        (serial_data == ~LINK_CABLE_MAGIC_BYTE_SYNC) ){
        connected = true;
    }
    if (use_internal_clock) {
        *rSB = LINK_CABLE_MAGIC_BYTE_SYNC;
    }
    *rSC = LINK_CABLE_ENABLE | use_internal_clock;
    return connected;
}

void main(void) {
    copy_tiles_to_vram();
    render_message_no_screen_flush(message_header);
    {
        bool did_write_to_ram = false;
        bool state_changed = true;
        bool is_leader = false;
        bool cartridge_state = false;
        bool link_cable_state = false;
        uint8_t button_state = 0;

        // uint8_t link_cable_connected_confidence = 0;

        // Infinite loop to keep the program running
        while (1) {
            flush_screen();

            bool new_is_leader = true;
            bool new_cartridge_state = true;
            bool new_link_cable_state = false;

            // Check Cartridge State
            for (uint8_t i = 0; i < 8; ++i){
                if (CARTRIDGE_LOGO[i] != CORPORATE_LOGO[i]){
                    new_cartridge_state = false;
                }
                if (CARTRIDGE_TITLE[i] != LEADER_CARTRIDGE_TITLE[i]){
                    new_is_leader = false;
                }
            }

            flush_screen();

            if(is_leader != new_is_leader){
                is_leader = new_is_leader;
                state_changed = true;
                message_list_t* message = is_leader? message_role_leader : message_role_worker;
                render_message(message);
            }
            
            if(cartridge_state != new_cartridge_state){
                cartridge_state = new_cartridge_state;
                state_changed = true;
                message_list_t* message = cartridge_state? 
                    message_cartridge_state_ok : 
                    message_cartridge_state_error;
                render_message(message);
            }

            if (cartridge_state) {
                new_link_cable_state = send_detect_link_cable_packet(is_leader);
            }
       
            if(link_cable_state != new_link_cable_state){
                link_cable_state = new_link_cable_state;
                state_changed = true;
                message_list_t* message = link_cable_state? 
                    message_link_cable_state_ok : 
                    message_link_cable_state_error;
                render_message(message);
            }

            if (is_leader && link_cable_state) {
                uint8_t new_button_state = read_button_state();
                if ((button_state & BTN_A) != (new_button_state & BTN_A)) {
                    state_changed = true;
                    button_state = new_button_state;
                }
            }

            if (state_changed){
                state_changed = false;
                flush_screen();
                clear_message();
                if(!cartridge_state){
                    render_message(message_insert_gbc_cartridge);
                    continue;
                }

                // Has cartridge, safe to write to ram at this point
                if(!did_write_to_ram){
                    copy_ram_functions_to_ram();
                    did_write_to_ram = true;
                }

                if(!link_cable_state){
                    if(is_leader){
                        render_message(message_idle_role_change_info);
                    }
                    else {
                        render_message(message_insert_gbc_link_cable);
                    }
                    continue;
                }

                if(is_leader){
                    if (!is_button_pressed(button_state, BTN_A)){
                        render_message(message_choose_action);
                        continue;
                    }
                } else {
                    render_message(message_waiting_for_leader);
                }

                // The VRAM version should only be used on GBA hardware
#ifdef VRAM_VERSION
                *rDevice_mode = DEVICE_MODE_AGB;
#else 
                *rDevice_mode = (*rDeviceModeBootup & BOOTUP_A_CGB) != 0? 
                    DEVICE_MODE_CGB :
                    DEVICE_MODE_GB;
#endif
                *rRole = is_leader? ROLE_LEADER : ROLE_WORKER;
                *rTransfer_mode = TRANSFER_MODE_NO_ACTION;
                *rTransfer_mode_remote = TRANSFER_MODE_NO_ACTION;

                if (is_leader){
                    if(is_button_pressed(button_state, BTN_START)){
                        *rTransfer_mode = TRANSFER_MODE_RESTORE_SAVE;
                    } else {
                        *rTransfer_mode = TRANSFER_MODE_BACKUP_SAVE;
                    }
                }

                run_in_parallel_to_screen(ram_fn_transfer_header);

                uint8_t transfer_mode = (*rTransfer_mode | *rTransfer_mode_remote);
                bool backup_save  = (0 != (transfer_mode & TRANSFER_MODE_BACKUP_SAVE));
                bool restore_save = (0 != (transfer_mode & TRANSFER_MODE_RESTORE_SAVE));

                clear_message();
                if(backup_save){
                    render_message(message_backing_up_save);
                } else if(restore_save){
                    render_message(message_restoring_save);
                }
                render_message(message_progress_bar);

                
                ram_fn_enable_cartridge_sram();
                run_in_parallel_to_screen(ram_fn_perform_transfer);

                clear_message_from_row(2);
                if (*rTransferError) {
                    render_message(message_transfer_error);
                } else {
                    render_message(message_transfer_done);
                }
                ram_fn_disable_cartridge_sram();

                // Busy wait at the end. User has to turn off console here
                while(1) {
                    flush_screen();
                }
            }
        }
    }
}

// Has to be last in order to be last in binary
#ifdef VRAM_VERSION
#define INCLUDE_BIN_DATA_DONT_USE_IN_HEADER
#include "ram_code_gbc.h"
#endif // VRAM_VERSION

// Important to have a non 0xFF in the end because bin2c strips away trailing data
// and main is hardcoded to be included last in the program
uint8_t end = 0xED;
