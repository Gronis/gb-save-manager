
#include "cartridges.h"

#include "version.h"

#include "area_ram.h"

// const cartridge_mode_t cartridge_mbc_3_ram_data = {
//     // bank_enable_addr                 (address, 8 msb)
//     0x00,
//     // bank_enable_value      
//     0x0A,
//     // bank_disable_value      
//     0x00,
//     // bank_selector_addr               (address, 8 msb)
//     0x40,
//     // bank_number_base: 4 rom banks
//     2,
//     // bank_data_addr_start             (inclusive, address, 8 msb)
//     0xA0,
//     // bank_data_addr_end               (exclusive, address, 8 msb)
//     0xC0,
//     // bank_enable_advanced_addr
//     0x60,
//     // bank_enable_advanced_value
//     0x01,
// };