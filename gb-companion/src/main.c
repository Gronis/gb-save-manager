#include "types.h"
#include "hardware.h"

#include "definitions.h"
#include "transfer.h"
#include "start.h"
#include "graphics.h"
#include "graphics_init.h"
#include "bitmaps.h"

// TODO: Move this to its own source file
////////////////////////////////////////////////////////////////////
#ifdef VRAM_VERSION
#define INCLUDE_BIN_DATA
#include "ram_code_gbc.h"
void copy_ram_functions_to_ram(void) { 
    uint8_t* src = ram_code;
    volatile uint8_t* dst = (uint8_t*)RAM_LOC;
    uint8_t* end = src + ram_code_length;
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
#endif 
////////////////////////////////////////////////////////////////////

const uint8_t corners_x[] = {
    0, SCREEN_COORDINATE_TILE_WIDTH, SCREEN_COORDINATE_TILE_WIDTH, 0
};

const uint8_t corners_y[] = {
    SCREEN_COORDINATE_TILE_HEIGHT - 1, 0, SCREEN_COORDINATE_TILE_HEIGHT, SCREEN_COORDINATE_TILE_HEIGHT
};

#define CARTRIDGE_TITLE ((char*)(0x0134))
const char LEADER_CARTRIDGE_TITLE[] = "GBSAVEMANA";
#define CARTRIDGE_LOGO ((char*)(0x0104))
const uint8_t CORPORATE_LOGO[] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73
};

void main(void) {
    render_message_no_screen_flush_call_only_before_rasterize(message_header);
    rasterize_all_bitmap_tiles_to_VRAM_call_only_once();
    render_message(message_progress_bar);
    {
        bool did_write_to_ram = false;
        bool state_changed = true;
        bool is_leader = false;
        bool cartridge_state = false;
        bool link_cable_state = false;

        // Infinite loop to keep the program running
        while (1) {
            flush_screen();

            bool new_is_leader = true;
            bool new_cartridge_state = true;
            bool new_link_cable_state = false;

            for (uint8_t i = 0; i < 10; ++i){
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
       
            if(link_cable_state != new_link_cable_state){
                link_cable_state = new_link_cable_state;
                state_changed = true;
                message_list_t* message = link_cable_state? 
                    message_link_cable_state_ok : 
                    message_link_cable_state_error;
                render_message(message);
            }

            if (state_changed){
                state_changed = false;
                flush_screen();
                clear_message();
                if(!cartridge_state){
                    render_message(message_insert_gbc_cartridge);
                    continue;
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
                if(!is_leader){
                    render_message(message_waiting_for_leader);
                    continue;
                }
                // TODO: here we should probably go to some progress bar handler
            }

            // tile_id = (tile_id + 1);
            // if (tile_id > 24){
            //     tile_id = 0;
            // }
            // toggle each corner except 1, flushing the screen in between to avoid flickering
            // for (uint8_t i = 0; i < 3; ++i){
            //     range_t* range = ((range_t*)(&tiles + tile_id));
            //     uint8_t width = range->end - range->start;
            //     flush_screen();
            //     set_tiles_row_repeat(corners_x[i], corners_y[i], *range, width);
            // }

            if(!did_write_to_ram){
                *((uint8_t*)_RAM+400) = 0x44;
                if (*((uint8_t*)_RAM+400) == 0x44) {
                    did_write_to_ram = true;
                    copy_ram_functions_to_ram();
                    run_in_parallel_to_screen(show_ram_is_working);
                }
            }

            // If RAM is unreadable, filcker slow, otherwise filcker fast.
            // RAM is unreadable when GBA is in GBC mode while no GBC cartridge is inserted. 
            // if (*((uint8_t*)_RAM+400) != 0x44) {
            //     // wait for 24 frames
            //     for (uint8_t i = 0; i < 12; ++i){
            //         flush_screen();
            //     }
            // }
            // flush_screen();
        }
    }
}
