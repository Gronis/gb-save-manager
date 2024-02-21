#include "transfer.h"
#include "hardware.h"
#include "graphics.h"
#include "bitmaps.h"
#include "cartridges.h"
#include "start.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

const uint8_t cartridge_sram_table[] = {
    0,  // 2: 8kB    1 bank
    2,  // 3: 32kB   4 banks
    4,  // 4: 128kB 16 banks
    3,  // 5: 64kB   8 banks
};

// First gen cartridge types
const cartridge_mode_t cartridge_mbc_1_ram_data = {
    0x00,   // bank_enable_addr                 (address, 8 msb)
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x40,   // bank_selector_addr               (address, 8 msb)
    0xA0,   // bank_data_addr_start             (inclusive, address, 8 msb)
    0xC0,   // bank_data_addr_end               (exclusive, address, 8 msb)
    0x60,   // bank_enable_advanced_addr
    0x01,   // bank_enable_advanced_value
    0xFF,   // bank_selector_bit_skip
};

// Mostly the same as MBC_1 except no advanced mode, just set enable again
const cartridge_mode_t cartridge_mbc_3_and_5_ram_data = {
    0x00,   // bank_enable_addr                 (address, 8 msb)
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x40,   // bank_selector_addr               (address, 8 msb)
    0xA0,   // bank_data_addr_start             (inclusive, address, 8 msb)
    0xC0,   // bank_data_addr_end               (exclusive, address, 8 msb)
    0x40,   // bank_enable_advanced_addr
    0x0A,   // bank_enable_advanced_value
    0xFF,   // bank_selector_bit_skip
};

// MBC_5 with rumble data uses bank selector register bit 3 (%0000_1000)
// to enable or disable the rumble motor. If we reach this bank number,
// we need to multiply by 2, skipping this bit entirely.
const cartridge_mode_t cartridge_mbc_5_rumble_ram_data = {
    0x00,   // bank_enable_addr                 (address, 8 msb)
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x40,   // bank_selector_addr               (address, 8 msb)
    0xA0,   // bank_data_addr_start             (inclusive, address, 8 msb)
    0xC0,   // bank_data_addr_end               (exclusive, address, 8 msb)
    0x40,   // bank_enable_advanced_addr
    0x0A,   // bank_enable_advanced_value
    0x08,   // bank_selector_bit_skip     <- bit 3 (%0000_1000) controls motor
};

uint8_t get_mbc_type(uint8_t cartridge_mbc_value){
    if (0x01 <= cartridge_mbc_value && cartridge_mbc_value <= 0x03 ) return MBC_1;
    // if (0x05 <= cartridge_mbc_value && cartridge_mbc_value <= 0x06 ) return MBC_2;
    if (0x0F <= cartridge_mbc_value && cartridge_mbc_value <= 0x13 ) return MBC_3;
    if (0x19 <= cartridge_mbc_value && cartridge_mbc_value <= 0x1B ) return MBC_5;
    if (0x1C <= cartridge_mbc_value && cartridge_mbc_value <= 0x1E ) return MBC_5_RUMBLE;
    return MBC_UNSUPPORTED;
}

cartridge_mode_t* get_cartridge_mode_ptr (uint8_t mbc_type) {
    return
        mbc_type == MBC_1?          &cartridge_mbc_1_ram_data :
        mbc_type == MBC_2?          &cartridge_mbc_1_ram_data :  // TODO
        mbc_type == MBC_3?          &cartridge_mbc_3_and_5_ram_data :
        mbc_type == MBC_5?          &cartridge_mbc_3_and_5_ram_data :
        mbc_type == MBC_5_RUMBLE?   &cartridge_mbc_5_rumble_ram_data :
        (cartridge_mode_t*)0x0000; // null, should never happen
}

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

void ram_fn_enable_cartridge_sram (void) {
    uint8_t mbc_type                    = get_mbc_type(*rCartridgeType_mode);
    cartridge_mode_t* cartridge_mode    = get_cartridge_mode_ptr(mbc_type);
    uint8_t* enable_addr                = as_addr(cartridge_mode->bank_enable_addr);
    uint8_t* enable_advanced_addr       = as_addr(cartridge_mode->bank_enable_advanced_addr);
    uint8_t enable_value                = cartridge_mode->bank_enable_value;
    uint8_t enable_advanced_value       = cartridge_mode->bank_enable_advanced_value;
    *enable_addr                        = enable_value;
    *enable_addr                        = enable_value;
    *enable_addr                        = enable_value;
    *enable_addr                        = enable_value;
    *enable_advanced_addr               = enable_advanced_value;
    *enable_advanced_addr               = enable_advanced_value;
    *enable_advanced_addr               = enable_advanced_value;
    *enable_advanced_addr               = enable_advanced_value;
}

void ram_fn_disable_cartridge_sram (void) {
    uint8_t mbc_type                    = get_mbc_type(*rCartridgeType_mode);
    cartridge_mode_t* cartridge_mode    = get_cartridge_mode_ptr(mbc_type);
    uint8_t* disable_addr               = as_addr(cartridge_mode->bank_enable_addr);
    uint8_t disable_value               = cartridge_mode->bank_disable_value;
    *disable_addr                       = disable_value;
    *disable_addr                       = disable_value;
    *disable_addr                       = disable_value;
    *disable_addr                       = disable_value;
}

// #define JUGGLE_SPI_MASTER
void ram_fn_perform_transfer(void) {

    *rTransferError = false;
    uint8_t timeout = 0x80;
    uint8_t transfer_mode = *rTransfer_mode | *rTransfer_mode_remote;
    bool backup_save  = (0 != (transfer_mode & TRANSFER_MODE_BACKUP_SAVE));
    bool restore_save = (0 != (transfer_mode & TRANSFER_MODE_RESTORE_SAVE));

    bool is_leader = *rRole == ROLE_LEADER;
    bool is_receiving_data = 
        (is_leader  && backup_save) ||
        (!is_leader && restore_save);

    // Emu don't like player 2 generating signals, so leader (p1) has to.
    bool use_internal_clock = is_leader;
    // bool use_internal_clock = is_receiving_data;

    uint8_t cartridge_sram_bank_size = 
        is_leader? 
            *rCartridgeSRAM_size_remote :
            *rCartridgeSRAM_size;

    // This means game does not have SRAM
    if (cartridge_sram_bank_size < 2){
        return;
    }

    uint8_t worker_cartridge_mbc_value = 
        is_leader? 
            *rCartridgeType_mode_remote :
            *rCartridgeType_mode;

    if (get_mbc_type(worker_cartridge_mbc_value) == MBC_UNSUPPORTED) {
        return;
    }

    uint8_t cartridge_mbc_value         = *rCartridgeType_mode;
    uint8_t mbc_type                    = get_mbc_type(cartridge_mbc_value);
    cartridge_mode_t* cartridge_mode    = get_cartridge_mode_ptr(mbc_type);
    uint8_t bank_number_base            = cartridge_sram_table[cartridge_sram_bank_size - 2];
    uint8_t* data_ptr                   = as_addr(cartridge_mode->bank_data_addr_end);
    uint8_t* data_ptr_end               = data_ptr;
    uint8_t next_bank_number            = 0;
    uint8_t visual_tile_row_index       = 0;

    // If we control the message flow, Wait some time before starting actual transfer
    // So that we know that the other end is ready for us when transfer starts.
    if (use_internal_clock){
        wait_n_cycles(0x4000);
    }

    // 64 packets fits 8kB of space (1 RAM bank).
    for (uint16_t packet_num = 0; packet_num < (64 << bank_number_base) && !(*rTransferError); ++packet_num) {
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
            if (data_ptr >= data_ptr_end) {
                data_ptr = as_addr(cartridge_mode->bank_data_addr_start);
                uint8_t* bank_selector = as_addr(cartridge_mode->bank_selector_addr);
                *bank_selector = next_bank_number;
                *bank_selector = next_bank_number;
                *bank_selector = next_bank_number;
                *bank_selector = next_bank_number;
                next_bank_number++;
                if(next_bank_number == cartridge_mode->bank_selector_bit_skip){
                    next_bank_number *= 2;
                }
            }

            // Send byte and update UI progress bar
            uint8_t msg_byte;
            if(!is_receiving_data){
                msg_byte = *data_ptr;
                checksum = checksum ^ msg_byte;
                send_byte(msg_byte, use_internal_clock);
            } else {
                send_last_byte(use_internal_clock);
            }
            try_update_progress_bar(packet_num >> bank_number_base);

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
                msg_byte = received_byte;
                checksum = checksum ^ msg_byte;

                // Write byte to ROM/RAM Bank
                *data_ptr = msg_byte;
                *data_ptr = msg_byte;
                *data_ptr = msg_byte;
                *data_ptr = msg_byte;
                if (*data_ptr != msg_byte){
                    *rTransferError = true;
                }
            }

            // Visualize transfered bytes as a tile
            *(_VRAM + n_tiles_total * 16 + ((visual_tile_row_index += 2) & 15)) = msg_byte;
            *(_SCRN1 + get_position_tile_index(12, 4)) = n_tiles_total;

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
