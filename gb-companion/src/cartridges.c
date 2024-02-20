
#include "cartridges.h"

#include "area_vram1.h"

const uint8_t cartridge_mbc_3_ram_data[] = {
    // bank_enable_addr                 (address, 8 msb)
    0x00,
    // bank_enable_value      
    0x0A,
    // bank_disable_value      
    0x00,
    // bank_selector_addr               (address, 8 msb)
    0x40,
    // bank_number_value_start          (inclusive)
    0x00,
    // bank_number_value_end            (exclusive)
    0x04,
    // bank_data_addr_start             (inclusive, address, 8 msb)
    0xA0,
    // bank_data_addr_end               (exclusive, address, 8 msb)
    0xC0,
    // bank_enable_advanced_addr
    0x60,
    // bank_enable_advanced_value
    0x01,
};