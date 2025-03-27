#ifndef TRANSFER_H
#define TRANSFER_H

#include "version.h"
#include "types.h"
#include "cartridges.h"
#include "tiles.h"

#define ROLE_WORKER                     0
#define ROLE_LEADER                     1

#define DEVICE_MODE_GB                  0
#define DEVICE_MODE_CGB                 1
#define DEVICE_MODE_AGB                 2

#define TRANSFER_MODE_NO_ACTION         0
#define TRANSFER_MODE_BACKUP_SAVE       1
#define TRANSFER_MODE_RESTORE_SAVE      2

// Store metadata at the end of RAM
#define rRole                           ((uint8_t*) (_RAMBANK - 1))
#define rDevice_mode                    ((uint8_t*) (_RAMBANK - 2))
#define rDevice_mode_remote             ((uint8_t*) (_RAMBANK - 3))
#define rTransfer_mode                  ((uint8_t*) (_RAMBANK - 4))
#define rTransfer_mode_remote           ((uint8_t*) (_RAMBANK - 5))

#define rMBC_mode                       ((uint8_t*) (0x0147))
#define rMBC_mode_remote                ((uint8_t*) (_RAMBANK - 6))
#define rROM_size                       ((uint8_t*) (0x0148))
#define rROM_size_remote                ((uint8_t*) (_RAMBANK - 7))
#define rSRAM_size                      ((uint8_t*) (0x0149))
#define rSRAM_size_remote               ((uint8_t*) (_RAMBANK - 8))

#define rTransferError                  ((uint8_t*) (_RAMBANK - 9))

#define LINK_CABLE_ENABLE               ((uint8_t)0x80)
#define LINK_CABLE_MAGIC_BYTE_SYNC      ((uint8_t)0xAA)

#define PACKET_SIZE                     64

#define as_addr(addr) ((uint8_t*)(((uint16_t)(addr)) << 8))

void ram_fn_enable_cartridge_sram (void);
void ram_fn_disable_cartridge_sram (void);

void ram_fn_transfer_header(void);
void ram_fn_perform_transfer(void);

#endif // TRANSFER_H
