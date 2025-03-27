
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
    .bank_enable_addr           = 0x00, // (address, 8 msb)
    .bank_enable_value          = 0x0A,
    .bank_disable_value         = 0x00,
    .bank_selector_addr         = 0x40, // (address, 8 msb)
    .bank_data_addr_start       = 0xA0, // (inclusive, address, 8 msb)
    .bank_data_addr_end         = 0xC0, // (exclusive, address, 8 msb)
    .bank_enable_advanced_addr  = 0x60,
    .bank_enable_advanced_value = 0x01,
    .bank_selector_bit_skip     = 0xFF,
    .base_size_override         = 0x00,

};

// Mostly the same as MBC_1 except no advanced mode, just set enable again
const cartridge_mode_t cartridge_mbc_2_ram_data = {
    .bank_enable_addr           = 0x00, // (address, 8 msb)
    .bank_enable_value          = 0x0A,
    .bank_disable_value         = 0x00,
    .bank_selector_addr         = 0x90, // <- write to dummy address
    .bank_data_addr_start       = 0xA0, // (inclusive, address, 8 msb)
    .bank_data_addr_end         = 0xA2, // (exclusive, address, 8 msb)
    .bank_enable_advanced_addr  = 0x00, // <- same as enable RAM mode
    .bank_enable_advanced_value = 0x0A,
    .bank_selector_bit_skip     = 0xFF,
    .base_size_override         = 0x03,
};

// Mostly the same as MBC_1 except no advanced mode, just set enable again
const cartridge_mode_t cartridge_mbc_3_and_5_ram_data = {
    .bank_enable_addr           = 0x00, // (address, 8 msb)
    .bank_enable_value          = 0x0A,
    .bank_disable_value         = 0x00,
    .bank_selector_addr         = 0x40, // (address, 8 msb)
    .bank_data_addr_start       = 0xA0, // (inclusive, address, 8 msb)
    .bank_data_addr_end         = 0xC0, // (exclusive, address, 8 msb)
    .bank_enable_advanced_addr  = 0x00, // <- same as enable RAM mode
    .bank_enable_advanced_value = 0x0A,
    .bank_selector_bit_skip     = 0xFF,
    .base_size_override         = 0x00,
};

// MBC_5 with rumble data uses bank selector register bit 3 (%0000_1000)
// to enable or disable the rumble motor. If we reach this bank number,
// we need to multiply by 2, skipping this bit entirely.
const cartridge_mode_t cartridge_mbc_5_rumble_ram_data = {
    .bank_enable_addr           = 0x00, // (address, 8 msb)
    .bank_enable_value          = 0x0A,
    .bank_disable_value         = 0x00,
    .bank_selector_addr         = 0x40, // (address, 8 msb)
    .bank_data_addr_start       = 0xA0, // (inclusive, address, 8 msb)
    .bank_data_addr_end         = 0xC0, // (exclusive, address, 8 msb)
    .bank_enable_advanced_addr  = 0x00, // <- same as enable RAM mode
    .bank_enable_advanced_value = 0x0A,
    .bank_selector_bit_skip     = 0x08, // <- bit 3 (%0000_1000) controls motor
    .base_size_override         = 0x00,
};

// Mostly the same as MBC_3 except with hardcoded 128kB SRAM size
const cartridge_mode_t cartridge_mbc_gb_cam_ram_data = {
    .bank_enable_addr           = 0x00, // (address, 8 msb)
    .bank_enable_value          = 0x0A,
    .bank_disable_value         = 0x00,
    .bank_selector_addr         = 0x40, // (address, 8 msb)
    .bank_data_addr_start       = 0xA0, // (inclusive, address, 8 msb)
    .bank_data_addr_end         = 0xC0, // (exclusive, address, 8 msb)
    .bank_enable_advanced_addr  = 0x00, // <- same as enable RAM mode
    .bank_enable_advanced_value = 0x0A,
    .bank_selector_bit_skip     = 0xFF,
    .base_size_override         = 0x0B, // <- 2^11 * PACKET_SIZE = 128kB
};

uint8_t get_mbc_type(uint8_t cartridge_mbc_value){
    if (0x01 <= cartridge_mbc_value && cartridge_mbc_value <= 0x03 ) return MBC_1;
    if (0x05 <= cartridge_mbc_value && cartridge_mbc_value <= 0x06 ) return MBC_2;
    if (0x0F <= cartridge_mbc_value && cartridge_mbc_value <= 0x13 ) return MBC_3;
    if (0x19 <= cartridge_mbc_value && cartridge_mbc_value <= 0x1B ) return MBC_5;
    if (0x1C <= cartridge_mbc_value && cartridge_mbc_value <= 0x1E ) return MBC_5_RUMBLE;
    if (0xFC <= cartridge_mbc_value && cartridge_mbc_value <= 0xFC ) return MBC_GB_CAMERA;
    return MBC_UNSUPPORTED;
}

cartridge_mode_t* get_cartridge_mode_ptr (uint8_t mbc_type) {
    const cartridge_mode_t* ram_data = (
        mbc_type == MBC_1?          &cartridge_mbc_1_ram_data :
        mbc_type == MBC_2?          &cartridge_mbc_2_ram_data :
        mbc_type == MBC_3?          &cartridge_mbc_3_and_5_ram_data :
        mbc_type == MBC_5?          &cartridge_mbc_3_and_5_ram_data :
        mbc_type == MBC_5_RUMBLE?   &cartridge_mbc_5_rumble_ram_data :
        mbc_type == MBC_GB_CAMERA?  &cartridge_mbc_gb_cam_ram_data :
        (cartridge_mode_t*)0x0000 // null, should never happen
    );
    // Since this rom lives in RAM it's ok to cast it to non-const
    // (we are not const strict in this codebase anyways)
    return ((cartridge_mode_t*)ram_data);
}
