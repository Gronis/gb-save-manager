#include "transfer.h"
#include "hardware.h"
#include "graphics.h"
#include "messages.h"
#include "cartridges.h"
#include "start.h"

// This will ensure code is put inside RAM
#include "area_ram.h"

void try_update_progress_bar(uint8_t progress){
    uint8_t* dst = (uint8_t*)(get_tile_position(4, 6) + progress / 8);
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
    timeout--;
    for(uint16_t i = 0; ((*rSC) & 0x80) != 0; ++i){
        if(i >= (((uint16_t)timeout) << 8)) {
            *rTransferError = true;
            break;
        }
    }
    return *rSB;
}

void wait_for_other_device(bool use_internal_clock) {
    uint8_t packet_to_send = use_internal_clock? (uint8_t)~LINK_CABLE_MAGIC_BYTE_SYNC : LINK_CABLE_MAGIC_BYTE_SYNC;
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
    send_byte(*rMBC_mode, use_internal_clock);
    *rMBC_mode_remote = recv_byte(0);
    send_byte(*rROM_size, use_internal_clock);
    *rROM_size_remote = recv_byte(0);
    send_byte(*rSRAM_size, use_internal_clock);
    *rSRAM_size_remote = recv_byte(0);
}

void ram_fn_transfer_header(void) {
    bool use_internal_clock = *rRole == ROLE_LEADER;
    wait_for_other_device(use_internal_clock);
    send_recv_header(use_internal_clock);
}

/// Activate cartridge ram memory so that it's read and writeable.
///
/// Power disruptions while cartridge ram is active can potentally cause memory
/// corruption.
void ram_fn_enable_cartridge_sram (void) {
    uint8_t mbc_type                    = get_mbc_type(*rMBC_mode);
    if (mbc_type == MBC_UNSUPPORTED) return;
    cartridge_mode_t* cartridge         = get_cartridge_mode_ptr(mbc_type);
    uint8_t* enable_addr                = as_addr(cartridge->bank_enable_addr);
    uint8_t enable_value                = cartridge->bank_enable_value;
    *enable_addr                        = enable_value;
    *enable_addr                        = enable_value;
    uint8_t* enable_advanced_addr       = as_addr(cartridge->bank_enable_advanced_addr);
    uint8_t enable_advanced_value       = cartridge->bank_enable_advanced_value;
    *enable_advanced_addr               = enable_advanced_value;
    *enable_advanced_addr               = enable_advanced_value;
}

/// Disables cartridge ram memory so that it's no longer read or writeable.
///
/// Make sure to do this when no longer working with cartridge ram in order
/// to protect its content in case of a sudden power failure.
void ram_fn_disable_cartridge_sram (void) {
    uint8_t mbc_type                    = get_mbc_type(*rMBC_mode);
    if (mbc_type == MBC_UNSUPPORTED) return;
    cartridge_mode_t* cartridge         = get_cartridge_mode_ptr(mbc_type);
    uint8_t* disable_addr               = as_addr(cartridge->bank_enable_addr);
    uint8_t disable_value               = cartridge->bank_disable_value;
    *disable_addr                       = disable_value;
    *disable_addr                       = disable_value;
}

void ram_fn_perform_transfer(void) {

    *rTransferError = false;
    uint8_t transfer_mode = *rTransfer_mode | *rTransfer_mode_remote;
    bool backup_save  = (TRANSFER_MODE_BACKUP_SAVE  == (transfer_mode & TRANSFER_MODE_BACKUP_SAVE));
    bool restore_save = (TRANSFER_MODE_RESTORE_SAVE == (transfer_mode & TRANSFER_MODE_RESTORE_SAVE));

    bool is_leader = *rRole == ROLE_LEADER;
    bool use_internal_clock = is_leader;
    bool is_receiving_data =
        (is_leader  && backup_save) ||
        (!is_leader && restore_save);

    uint8_t worker_sram_size;
    uint8_t worker_mbc_mode;
    if (is_leader){
        worker_sram_size                = *rSRAM_size_remote;
        worker_mbc_mode                 = *rMBC_mode_remote;
    } else {
        worker_sram_size                = *rSRAM_size;
        worker_mbc_mode                 = *rMBC_mode;
    }

    // This means game does not have SRAM
    if (worker_sram_size < 2) return;

    uint8_t worker_mbc_type             = get_mbc_type(worker_mbc_mode);
    if (worker_mbc_type == MBC_UNSUPPORTED) return;

    uint8_t sram_size_base              = cartridge_sram_table[worker_sram_size - 2];
    uint16_t max_num_of_pkts            = 64 << sram_size_base;

    // Some MBC types override the bank size found in the cartridge header
    cartridge_mode_t* worker_cartridge  = get_cartridge_mode_ptr(worker_mbc_type);
    if (worker_cartridge->base_size_override != 0){
        max_num_of_pkts                 = 1 << worker_cartridge->base_size_override;
    }

    uint16_t progress_bytes_per_inc     = max_num_of_pkts;
    uint16_t progress_bytes_counter     = 0;
    uint8_t progress_ui                 = 0;
    uint8_t next_bank_number            = 0;
    uint8_t visual_tile_row_index       = 0;

    cartridge_mode_t* cartridge         = get_cartridge_mode_ptr(get_mbc_type(*rMBC_mode));
    uint8_t* data_ptr                   = as_addr(cartridge->bank_data_addr_end);
    uint8_t* data_ptr_end               = data_ptr;
    bool inc_data_ptr                   = true;

    // If we control the message flow, Wait some time before starting actual transfer
    // So that we know that the other end is ready for us when transfer starts.
    if (use_internal_clock){
        wait_n_cycles(0x4000);
    }

    uint8_t timeout = 0x80;
    // 64 packets fits 8kB of space (1 RAM bank).
    for (uint16_t pkt_num = 0; pkt_num < max_num_of_pkts && !(*rTransferError); ++pkt_num) {
        uint8_t checksum = 0;

        // Send packet number
        {
            send_byte(pkt_num, use_internal_clock);
            uint16_t received_pkt_num = recv_byte(timeout);
            send_byte(pkt_num >> 8, use_internal_clock);
            received_pkt_num |= recv_byte(timeout) << 8;

            // Remote wants to resend previous packet
            if (is_receiving_data && pkt_num != received_pkt_num){
                *rTransferError = true;
            }
        }

        for (uint8_t i = 0; i < PACKET_SIZE && !(*rTransferError); ++i, data_ptr+=inc_data_ptr){

            // Progress ROM/RAM Bank if necessary
            if (data_ptr >= data_ptr_end) {
                data_ptr = as_addr(cartridge->bank_data_addr_start);
                uint8_t* bank_selector = as_addr(cartridge->bank_selector_addr);
                *bank_selector = next_bank_number;
                *bank_selector = next_bank_number;
                next_bank_number++;
                if(next_bank_number == cartridge->bank_selector_bit_skip){
                    next_bank_number *= 2;
                }
            }

            // Send byte and update UI progress bar
            uint8_t msg_byte;
            if(!is_receiving_data){
                if (is_nibble_mode(max_num_of_pkts)){
                    if (backup_save) {
                        // Join two nibbles into one byte
                        msg_byte = *data_ptr & 0x0F;
                        data_ptr++;
                        msg_byte |= *data_ptr << 4;
                    } else {
                        // Send a nibble, swapping between upper and lower bits
                        inc_data_ptr = !inc_data_ptr;
                        if (inc_data_ptr) {
                            msg_byte = *data_ptr >> 4;
                        } else {
                            goto write_msg_byte;
                        }
                    }
                } else {
                    write_msg_byte:
                    msg_byte = *data_ptr;
                }
                checksum = checksum ^ msg_byte;
                send_byte(msg_byte, use_internal_clock);
            } else {
                send_last_byte(use_internal_clock);
            }

            if (progress_bytes_counter++ >= progress_bytes_per_inc) {
                progress_bytes_counter = 0;
                progress_ui++;
            }
            try_update_progress_bar(progress_ui);

            // If we are not juggling spi clock leader role back and forth,
            // wait to make sure worker is not behind and is missing packets.
            if (use_internal_clock){
                wait_n_cycles(0x0110);
            }

            // Receive byte and compute checksum
            uint8_t received_byte = recv_byte(timeout);
            if (is_receiving_data){
                msg_byte = received_byte;
                checksum = checksum ^ msg_byte;
                *data_ptr = msg_byte;
                *data_ptr = msg_byte;
                if (*data_ptr != msg_byte){
                    *rTransferError = true;
                }
            }

            // Visualize transfered bytes as a tile
            *(_VRAM + n_tiles_total * 16 + ((visual_tile_row_index += 2) & 0x0F)) = msg_byte;
            *as_u8_ptr(get_tile_position(12, 4)) = n_tiles_total;
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
