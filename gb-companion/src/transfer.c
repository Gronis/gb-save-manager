#include "transfer.h"
#include "hardware.h"
#include "graphics.h"
#include "data.h"
#include "start.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

void try_update_progress_bar(uint8_t progress){
    uint8_t* dst = _SCRN1 + get_position_tile_index(3, 6) + progress / 8;
    uint8_t tile = pb_start_offset + (progress & 7);
    if (*rLY <= 144) {
        *dst = tile;
    }
}

void send_last_byte(bool use_internal_clock){
    *rSC = LINK_CABLE_ENABLE | use_internal_clock;
}

void send_byte(uint8_t byte, bool use_internal_clock){
    *rSB = byte;
    send_last_byte(use_internal_clock);
}

uint8_t recv_byte(uint8_t timeout){
    timeout--;
    for(uint16_t i = 0; i < ((((uint16_t)timeout) << 8) | timeout) && ((*rSC) & 0x80) != 0; ++i);
    return *rSB;
}

void wait_for_other_device(bool use_internal_clock) {
    uint8_t packet_to_send = use_internal_clock? ~LINK_CABLE_MAGIC_PACKET_SYNC : LINK_CABLE_MAGIC_PACKET_SYNC;
    uint8_t packet_to_receive = ~packet_to_send;
    uint8_t received_packet;
    do {
        send_byte(packet_to_send, use_internal_clock);
        received_packet = recv_byte(0);
    } while (received_packet != packet_to_receive);
}

void send_recv_header(bool use_internal_clock){
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
    bool use_internal_clock = *rRole == ROLE_LEADER;
    wait_for_other_device(use_internal_clock);
    send_recv_header(use_internal_clock);
}

#define PACKET_SIZE 128
void ram_fn_perform_transfer(void) {

    uint8_t transfer_mode = *rTransfer_mode | *rTransfer_mode_remote;
    bool backup_save  = (0 != (transfer_mode & TRANSFER_MODE_BACKUP_SAVE));
    bool restore_save = (0 != (transfer_mode & TRANSFER_MODE_RESTORE_SAVE));

    bool is_leader = *rRole == ROLE_LEADER;
    bool should_write_data_to_cartridge = 
        (is_leader  && backup_save) ||
        (!is_leader && restore_save);

    bool use_internal_clock = should_write_data_to_cartridge;

    // if(!should_write_data_to_cartridge) return;

    // TODO: grab this from MBC data
    uint8_t* data_ptr = (uint8_t*)(should_write_data_to_cartridge? 0xD000 : 0xC000);

    uint8_t vi = 0;

    // If we control the message flow, Wait some time before starting actual transfer
    // So that we know that the other end is ready for us when transfer starts.
    if (use_internal_clock){
        wait_n_cycles(0x4000);
    }

    for (uint16_t packet_num = 0; packet_num < 32; ++packet_num) {
        uint8_t checksum = 0;
        for (uint8_t i = 0; i < PACKET_SIZE; ++i, ++data_ptr){
            if(!should_write_data_to_cartridge){
                uint8_t msg = *data_ptr;
                send_byte(msg, use_internal_clock);
            } else {
                send_last_byte(use_internal_clock);
            }
            try_update_progress_bar(packet_num);

            // If we control the link cable bus, wait a small amount of time to make sure
            // other end always keeps up. Use this if clock swapping does not work.
            // if (use_internal_clock){
            //     wait_n_cycles(0x200);
            // }
            uint8_t received_byte = recv_byte(0);

            // Visualize transfered bytes
            *(_VRAM + tiles_end * 16 + ((vi+=2) & 15)) = received_byte;
            *(_SCRN1 + get_position_tile_index(12, 4)) = tiles_end;

            if (should_write_data_to_cartridge){
                *data_ptr = received_byte;
            }
            // This does not work in emu because of random turbo mode bug
            use_internal_clock = !use_internal_clock;
        }
    }
}
