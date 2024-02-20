#ifndef TRANSFER_H
#define TRANSFER_H

#include "version.h"
#include "types.h"

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

#define rCartridgeType_mode             ((uint8_t*) (0x0147))
#define rCartridgeType_mode_remote      ((uint8_t*) (_RAMBANK - 6))
#define rCartridgeROM_size              ((uint8_t*) (0x0148))
#define rCartridgeROM_size_remote       ((uint8_t*) (_RAMBANK - 7))
#define rCartridgeSRAM_size             ((uint8_t*) (0x0149))
#define rCartridgeSRAM_size_remote      ((uint8_t*) (_RAMBANK - 8))

#define rTransferError                  ((uint8_t*) (_RAMBANK - 9))

#define LINK_CABLE_ENABLE               0x80
#define LINK_CABLE_MAGIC_BYTE_SYNC      0xAA

#define PACKET_SIZE                     128

typedef struct {
    uint8_t bank_enable_addr;
    uint8_t bank_enable_value;
    uint8_t bank_disable_value;
    uint8_t bank_selector_addr;
    uint8_t bank_number_value_start;
    uint8_t bank_number_value_end;
    uint8_t bank_data_addr_start;
    uint8_t bank_data_addr_end;
    uint8_t bank_enable_advanced_addr;
    uint8_t bank_enable_advanced_value;
} cartridge_mode_t;

#define as_addr(addr) ((uint8_t*)(((uint16_t)(addr)) << 8))

void ram_fn_transfer_header(void);
void ram_fn_perform_transfer(void);

#endif // TRANSFER_H

