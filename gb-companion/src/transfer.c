#include "transfer.h"
#include "hardware.h"
#include "graphics.h"
#include "data.h"
#include "start.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

void wait_for_vblank_or_recv_byte() {
    bool done;
    while (!(done = *rLY == 144));
}

void send_byte(uint8_t byte, uint8_t use_internal_clock){
    *rSB = byte;
    *_VRAM = byte;
    *rSC = LINK_CABLE_ENABLE | use_internal_clock;
}

uint8_t recv_byte(uint8_t timeout){
    timeout--;
    for(uint16_t i = 0; i < ((((uint16_t)timeout) << 8) | timeout) && ((*rSC) & 0x80) != 0; ++i);
    return *rSB;
}

void wait_for_other_device(uint8_t use_internal_clock) {
    uint8_t packet_to_send = use_internal_clock? ~LINK_CABLE_MAGIC_PACKET_SYNC : LINK_CABLE_MAGIC_PACKET_SYNC;
    uint8_t packet_to_receive = ~packet_to_send;
    uint8_t received_packet;
    do {
        send_byte(packet_to_send, use_internal_clock);
        received_packet = recv_byte(0);
    } while (received_packet != packet_to_receive);
}

void send_recv_header(uint8_t use_internal_clock){
    send_byte(*rDevice_mode, use_internal_clock);
    *rDevice_mode_remote = recv_byte(0);
    send_byte(*rTransfer_mode, use_internal_clock);
    *rTransfer_mode_remote = recv_byte(0);
    send_byte(*rCartridgeType_mode, use_internal_clock);
    *rCartridgeType_mode_remote = recv_byte(0);
    send_byte(*rCartridgeROM_size, use_internal_clock);
    *rCartridgeROM_size_remote = recv_byte(0);
    send_byte(*rCartridgeSRAM_size, use_internal_clock);
    *rCartridgeSRAM_size_remote = recv_byte(0);
}

void ram_fn_transfer_header(void) {
    uint8_t use_internal_clock = *rRole == ROLE_LEADER;
    wait_for_other_device(use_internal_clock);
    send_recv_header(use_internal_clock);
}

void try_update_progress_bar(uint8_t progress){
    uint8_t* dst = _SCRN1 + get_position_tile_index(3, 6) + progress / 8;
    uint8_t tile = tiles[(pb_start_tile_index + 1) + (progress & 7) * 2];
    if (tile) {
        *dst = tile;
    }
}

void ram_fn_perform_transfer(void) {

    // for(uint8_t i = 0; i < 72; ++i){
    //     // wait_for_vblank_or_recv_byte();
    //     try_update_progress_bar(i);
    // }

    // Send rom metadata (name, rom-banks info etc)
    // if (rRole == )

    // Detect if we should send or receive data

    // Send/Receive data, verify each packet with crc??
}
