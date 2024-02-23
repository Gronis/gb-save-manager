#ifndef CARTRIDGES_H
#define CARTRIDGES_H

#include "types.h"

typedef struct {
    uint8_t bank_enable_addr;               // (address, 8 msb)
    uint8_t bank_enable_value;
    uint8_t bank_disable_value;
    uint8_t bank_selector_addr;             // (address, 8 msb)
    uint8_t bank_data_addr_start;           // (inclusive, address, 8 msb)
    uint8_t bank_data_addr_end;             // (exclusive, address, 8 msb)
    uint8_t bank_enable_advanced_addr;
    uint8_t bank_enable_advanced_value;
    uint8_t bank_selector_bit_skip;
    uint8_t base_size_override;
} cartridge_mode_t;

#define RUMBLE          0x10

#define MBC_UNSUPPORTED 0
#define MBC_1           1
#define MBC_2           2
#define MBC_3           3
#define MBC_5           5
#define MBC_5_RUMBLE    (MBC_5 | RUMBLE)

extern const uint8_t cartridge_sram_table[];
extern const cartridge_mode_t cartridge_mbc_1_ram_data;
extern const cartridge_mode_t cartridge_mbc_2_ram_data;
extern const cartridge_mode_t cartridge_mbc_3_and_5_ram_data;
extern const cartridge_mode_t cartridge_mbc_5_rumble_ram_data;
uint8_t get_mbc_type(uint8_t cartridge_mbc_value);
cartridge_mode_t* get_cartridge_mode_ptr (uint8_t mbc_type);

#define is_nibble_mode(max_num_of_pkts) ((((uint8_t)max_num_of_pkts) & 0x3F) != 0)

#endif // CARTRIDGES_H