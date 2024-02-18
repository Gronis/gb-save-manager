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

#define LINK_CABLE_ENABLE               0x80
#define LINK_CABLE_MAGIC_PACKET_SYNC    0xAA

void ram_fn_transfer_header(void);
void ram_fn_perform_transfer(void);

#endif // TRANSFER_H

