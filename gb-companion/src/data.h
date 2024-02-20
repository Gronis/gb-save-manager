#ifndef DATA_H
#define DATA_H

#include "types.h"

extern const uint8_t vram1_code[];
extern const uint8_t ram_code[];

#include "vram1_code_gbc.h"

#ifdef VRAM_VERSION
#include "ram_code_gbc.h"
#endif 

#endif // DATA_H