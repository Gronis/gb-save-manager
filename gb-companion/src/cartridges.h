#ifndef CARTRIDGES_H
#define CARTRIDGES_H

#include "types.h"

typedef struct {
    uint8_t bank_enable_addr;
    uint8_t bank_enable_value;
    uint8_t bank_disable_value;
    uint8_t bank_selector_addr;
    uint8_t bank_number_base;
    uint8_t bank_data_addr_start;
    uint8_t bank_data_addr_end;
    uint8_t bank_enable_advanced_addr;
    uint8_t bank_enable_advanced_value;
} cartridge_mode_t;

#define cartridge_mbc_3_ram             (&cartridge_mbc_3_ram_data)
extern const cartridge_mode_t           cartridge_mbc_3_ram_data;

#endif // CARTRIDGES_H