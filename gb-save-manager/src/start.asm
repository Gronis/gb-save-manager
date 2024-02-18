.include "hardware.inc"

.globl _main
.globl _STACK_PTR

rDeviceModeBootup       .equ 0xFFFC     ; This register holds if we are in AGB, CGB, GB, etc mode

.area _ENTRYPOINT
entrypoint:
    nop
    jp start

.area _CODE

start:                                  ; Detect AGB, CGB and GB mode before doing anything else.
    ld  hl, #rDeviceModeBootup
    rlc b
    or  b
    ld  (hl), a
    jp _main

.globl _execute_vram_code
_execute_vram_code:
    jp _VRAM
