.include "hardware.inc"

.globl _main
.globl _STACK_PTR

.globl _rDeviceModeBootup
_rDeviceModeBootup       .equ 0xFFFC    ; This register holds if we are in AGB, CGB, GB, etc mode

.area _ENTRYPOINT
entrypoint:
    nop
    jp start

.area _CODE

start:
    and #0xFD                           ; reg a hold GB/CGB/GB Pocket. Reset bit 2 for AGB mode.
    rlc b                               ; reg b hold if we are in AGB mode, move it to bit 2.
    or  b                               ; and join with reg a which holds if we are in GB/CGB mode
    ld  hl, #_rDeviceModeBootup
    ld  (hl), a
    jp _main

.globl _execute_code
_execute_code:
    jp _VRAM

