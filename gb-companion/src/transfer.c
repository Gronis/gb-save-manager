#include "transfer.h"
#include "hardware.h"
#include "graphics.h"
#include "start.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

void wait_for_vblank_or_link_cable() {
    bool done;
    while (!(done = *rLY == 144));
}

void show_ram_is_working() {
    range_t* range = ((range_t*)(&tiles + pb_8_tile_index));
    wait_for_vblank_or_link_cable();
    set_tiles_row_repeat(14, 0, *range, 1);
}

// typedef struct {

// } packet_t;

void send_message(uint8_t* data, uint8_t len){
}

void send_byte(uint8_t byte, uint8_t use_internal_clock) {
    *rSB = byte;
    *rSC = LINK_CABLE_ENABLE | use_internal_clock;
    // Wait for transfer to complete
    while((*rSC & 0x80) != 0);
}

void wait_for_other_device(void) {
    // uint8_t use_internal_clock = true;
    uint8_t use_internal_clock = rWorker;
    uint8_t magic_byte = 0x18;
    while(*rSB != magic_byte){
        send_byte(magic_byte, use_internal_clock);
    }
}

void ram_fn_transfer_header(void) {
    // wait_for_other_device();
    uint8_t use_internal_clock = rLeader;
    retry:
    if (use_internal_clock) {
        send_byte(LINK_CABLE_MAGIC_PACKET, use_internal_clock);
        if(*rSB != ~LINK_CABLE_MAGIC_PACKET){
            goto retry;
        }
        // send_byte(0x23, use_internal_clock);
        // if(*rSB != 0x12){
        //     while(1);
        // }
    } else {
        send_byte(~LINK_CABLE_MAGIC_PACKET, use_internal_clock);
        if(*rSB != LINK_CABLE_MAGIC_PACKET){
            goto retry;
        }
        // send_byte(0x23, use_internal_clock);
        // if(*rSB != 0x23){
        //     while(1);
        // }
    }
    show_ram_is_working();
}