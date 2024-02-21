#ifndef CARTRIDGES_H
#define CARTRIDGES_H

#include "types.h"

typedef struct {
    uint8_t bank_enable_addr;
    uint8_t bank_enable_value;
    uint8_t bank_disable_value;
    uint8_t bank_selector_addr;
    uint8_t bank_data_addr_start;
    uint8_t bank_data_addr_end;
    uint8_t bank_enable_advanced_addr;
    uint8_t bank_enable_advanced_value;
    uint8_t bank_selector_bit_skip;
} cartridge_mode_t;

#define RUMBLE          0x10

#define MBC_UNSUPPORTED 0
#define MBC_1           1
#define MBC_2           2
#define MBC_3           3
#define MBC_5           5
#define MBC_5_RUMBLE    (MBC_5 | RUMBLE)

#endif // CARTRIDGES_H