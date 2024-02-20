#include "transfer.h"
#include "hardware.h"
#include "graphics.h"
#include "data.h"
#include "start.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

void try_update_progress_bar(uint8_t progress){
    uint8_t* dst = _SCRN1 + get_position_tile_index(4, 6) + progress / 8;
    uint8_t tile = pb_start_offset + (progress & 7);
    if (*rLY <= 144) {
        *dst = tile;
    }
}

void send_last_byte(bool use_internal_clock){
    // uint8_t fast_mode = (*rDevice_mode != DEVICE_MODE_GB) << 1;
    *rSC = LINK_CABLE_ENABLE | use_internal_clock; // | fast_mode;
}

void send_byte(uint8_t byte, bool use_internal_clock){
    *rSB = byte;
    send_last_byte(use_internal_clock);
}

uint8_t recv_byte(uint8_t timeout){
    bool no_timeout_occured = true;
    timeout--;
    for(uint16_t i = 0; (no_timeout_occured = i < (((uint16_t)timeout) << 8)) && ((*rSC) & 0x80) != 0; ++i);
    if (!no_timeout_occured){
        *rTransferError = true;
    }
    return *rSB;
}

void wait_for_other_device(bool use_internal_clock) {
    uint8_t packet_to_send = use_internal_clock? ~LINK_CABLE_MAGIC_BYTE_SYNC : LINK_CABLE_MAGIC_BYTE_SYNC;
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

// #define JUGGLE_SPI_MASTER
void ram_fn_perform_transfer(void) {

    uint8_t timeout = 0x80;
    uint8_t transfer_mode = *rTransfer_mode | *rTransfer_mode_remote;
    bool backup_save  = (0 != (transfer_mode & TRANSFER_MODE_BACKUP_SAVE));
    bool restore_save = (0 != (transfer_mode & TRANSFER_MODE_RESTORE_SAVE));

    bool is_leader = *rRole == ROLE_LEADER;
    bool is_receiving_data = 
        (is_leader  && backup_save) ||
        (!is_leader && restore_save);

    bool use_internal_clock = is_receiving_data;

    // Since cartridge mode data is in VRAM, copy it before proceeding.
    cartridge_mode_t* cartridge_mode = cartridge_mbc_3_ram;
    // TODO use bank_number_value_end to determine length of data
    uint8_t bank_enable_addr            = cartridge_mode->bank_enable_addr;
    uint8_t bank_enable_value           = cartridge_mode->bank_enable_value;
    uint8_t bank_selector_addr          = cartridge_mode->bank_selector_addr;
    uint8_t bank_number_value_start     = cartridge_mode->bank_number_value_start;
    uint8_t bank_number_value_end       = cartridge_mode->bank_number_value_end;
    uint8_t bank_data_addr_start        = cartridge_mode->bank_data_addr_start;
    uint8_t bank_data_addr_end          = cartridge_mode->bank_data_addr_end;
    uint8_t bank_enable_advanced_addr   = cartridge_mode->bank_enable_advanced_addr;
    uint8_t bank_enable_advanced_value  = cartridge_mode->bank_enable_advanced_value;

    *as_addr(bank_enable_addr)          = bank_enable_value;
    *as_addr(bank_enable_advanced_addr) = bank_enable_advanced_value;

    uint8_t* data_ptr =         as_addr(bank_data_addr_end);
    uint8_t* data_ptr_end =     as_addr(bank_data_addr_end);
    uint8_t  next_bank_number = bank_number_value_start;

    uint8_t visual_tile_row_index = 0;
    *rTransferError = false;

    // If we control the message flow, Wait some time before starting actual transfer
    // So that we know that the other end is ready for us when transfer starts.
    if (use_internal_clock){
        wait_n_cycles(0x4000);
    }

    for (uint16_t packet_num = 0; packet_num < 256 && !(*rTransferError); ++packet_num) {
        uint8_t checksum = 0;

        // Send packet number
        {
            send_byte(packet_num, use_internal_clock);
            uint16_t received_packet_num = recv_byte(timeout);
            send_byte(packet_num >> 8, use_internal_clock);
            received_packet_num |= recv_byte(timeout) << 8;

            // Remote wants to resend previous packet
            if (is_receiving_data && packet_num != received_packet_num){
                *rTransferError = true;
            }
        }

        for (uint8_t i = 0; i < PACKET_SIZE && !(*rTransferError); ++i, ++data_ptr){

            // Progress ROM/RAM Bank if necessary
            if (data_ptr == data_ptr_end) {
                data_ptr = as_addr(bank_data_addr_start);
                *as_addr(bank_selector_addr) = next_bank_number++;
            }

            // Send byte and update UI progress bar
            if(!is_receiving_data){
                uint8_t msg = *data_ptr;
                checksum = checksum ^ msg;
                send_byte(msg, use_internal_clock);
            } else {
                send_last_byte(use_internal_clock);
            }
            try_update_progress_bar(packet_num / 4);

#ifndef JUGGLE_SPI_MASTER
            // If we are not juggling spi clock leader role back and forth,
            // wait to make sure worker is not behind and is missing packets.
            if (use_internal_clock){
                wait_n_cycles(0x0110);
            }
#endif

            // Receive byte and compute checksum
            uint8_t received_byte = recv_byte(timeout);
            if (is_receiving_data){
                checksum = checksum ^ received_byte;
            }

            // Visualize transfered bytes as a tile
            *(_VRAM + n_tiles_total * 16 + ((visual_tile_row_index += 2) & 15)) = ~received_byte;
            *(_SCRN1 + get_position_tile_index(12, 4)) = n_tiles_total;

            // Write byte to ROM/RAM Bank
            if (is_receiving_data){
                *data_ptr = received_byte;
            }

            // This does not work in emu because of random turbo mode bug
#ifdef JUGGLE_SPI_MASTER
            use_internal_clock = !use_internal_clock;
#endif
        }

        // Exchange checksum to verify that packet was transfered correctly
        {
            send_byte(checksum, use_internal_clock);
            uint8_t received_byte = recv_byte(timeout);

            // If we are sending and checksum from remote is incorrect, error out
            if (!is_receiving_data && received_byte != checksum){
                *rTransferError = true;
            }
        }
    }
}
