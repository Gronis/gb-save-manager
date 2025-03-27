
#include "cartridges.h"
#include "version.h"

// This will ensure code is put inside RAM
#include "area_ram.h"

// One packet is 4kB or half a memory bank. This is a lookup table to get the
// actual size from the cartridge header. So 2 to the power of X should be
// the size in bytes where X comes from this table.
const uint8_t cartridge_sram_table[] = {
    1,      // 2: 8kB    1 bank
    3,      // 3: 32kB   4 banks
    5,      // 4: 128kB 16 banks
    4,      // 5: 64kB   8 banks
};

// First gen cartridge types
const cartridge_mode_t cartridge_mbc_1_ram_data = {
    0x00,   // bank_enable_addr
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x40,   // bank_selector_addr
    0xA0,   // bank_data_addr_start
    0xC0,   // bank_data_addr_end
    0x60,   // bank_enable_advanced_addr
    0x01,   // bank_enable_advanced_value
    0xFF,   // bank_selector_bit_skip
    0x00,   // base_size_override
};

// Mostly the same as MBC_1 except no advanced mode, just set enable again
const cartridge_mode_t cartridge_mbc_2_ram_data = {
    0x00,   // bank_enable_addr
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x90,   // bank_selector_addr           <- write to dummy address
    0xA0,   // bank_data_addr_start
    0xA2,   // bank_data_addr_end
    0x00,   // bank_enable_advanced_addr    <- same as enable RAM mode
    0x0A,   // bank_enable_advanced_value
    0xFF,   // bank_selector_bit_skip
    0x03,   // base_size_override           <- 2^3 * PACKET_SIZE = 512 half bytes
};

// Mostly the same as MBC_1 except no advanced mode, just set enable again
const cartridge_mode_t cartridge_mbc_3_and_5_ram_data = {
    0x00,   // bank_enable_addr
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x40,   // bank_selector_addr
    0xA0,   // bank_data_addr_start
    0xC0,   // bank_data_addr_end
    0x00,   // bank_enable_advanced_addr    <- same as enable RAM mode
    0x0A,   // bank_enable_advanced_value
    0xFF,   // bank_selector_bit_skip
    0x00,   // base_size_override
};

// MBC_5 with rumble data uses bank selector register bit 3 (%0000_1000)
// to enable or disable the rumble motor. If we reach this bank number,
// we need to multiply by 2, skipping this bit entirely.
const cartridge_mode_t cartridge_mbc_5_rumble_ram_data = {
    0x00,   // bank_enable_addr
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x40,   // bank_selector_addr
    0xA0,   // bank_data_addr_start
    0xC0,   // bank_data_addr_end
    0x00,   // bank_enable_advanced_addr    <- same as enable RAM mode
    0x0A,   // bank_enable_advanced_value
    0x08,   // bank_selector_bit_skip       <- bit 3 (%0000_1000) controls motor
    0x00,   // base_size_override
};

// Mostly the same as MBC_1 except no advanced mode, just set enable again
const cartridge_mode_t cartridge_mbc_gb_cam_ram_data = {
    0x00,   // bank_enable_addr
    0x0A,   // bank_enable_value      
    0x00,   // bank_disable_value      
    0x40,   // bank_selector_addr
    0xA0,   // bank_data_addr_start
    0xC0,   // bank_data_addr_end
    0x00,   // bank_enable_advanced_addr    <- same as enable RAM mode
    0x0A,   // bank_enable_advanced_value
    0xFF,   // bank_selector_bit_skip
    0x0B,   // base_size_override           <- 2^11 * PACKET_SIZE = 128kB
};

uint8_t get_mbc_type(uint8_t cartridge_mbc_value){
    if (0x01 <= cartridge_mbc_value && cartridge_mbc_value <= 0x03 ) return MBC_1;
    if (0x05 <= cartridge_mbc_value && cartridge_mbc_value <= 0x06 ) return MBC_2;
    if (0x0F <= cartridge_mbc_value && cartridge_mbc_value <= 0x13 ) return MBC_3;
    if (0x19 <= cartridge_mbc_value && cartridge_mbc_value <= 0x1B ) return MBC_5;
    if (0x1C <= cartridge_mbc_value && cartridge_mbc_value <= 0x1E ) return MBC_5_RUMBLE;
    if (0xFC <= cartridge_mbc_value && cartridge_mbc_value <= 0xFC ) return MBC_GB_CAMERA; // Treat Gameboy camera as MBC3
    return MBC_UNSUPPORTED;
}

cartridge_mode_t* get_cartridge_mode_ptr (uint8_t mbc_type) {
    return
        mbc_type == MBC_1?          &cartridge_mbc_1_ram_data :
        mbc_type == MBC_2?          &cartridge_mbc_2_ram_data :
        mbc_type == MBC_3?          &cartridge_mbc_3_and_5_ram_data :
        mbc_type == MBC_5?          &cartridge_mbc_3_and_5_ram_data :
        mbc_type == MBC_5_RUMBLE?   &cartridge_mbc_5_rumble_ram_data :
        mbc_type == MBC_GB_CAMERA?  &cartridge_mbc_gb_cam_ram_data :
        (cartridge_mode_t*)0x0000; // null, should never happen
}
