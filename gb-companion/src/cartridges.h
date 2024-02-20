#ifndef CARTRIDGES_H
#define CARTRIDGES_H

#include "types.h"

#define cartridge_mbc_3_ram             ((cartridge_mode_t*)(&cartridge_mbc_3_ram_data))

extern const uint8_t                    cartridge_mbc_3_ram_data[];

#endif // CARTRIDGES_H