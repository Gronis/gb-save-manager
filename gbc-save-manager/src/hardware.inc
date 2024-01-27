;*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;* ! NOTE !
;* 
;* This hardware.inc is not for RGBDS. This file is adapted to work with sdcc 
;* assembler. DO NOT COPY THIS FILE AND USE IT IN A RGBDS PROJECT. Find another
;* hardware.inc file instead.
;* 
;*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;*
;* Gameboy Hardware definitions
;*
;* Based on Jones' hardware.inc
;* And based on Carsten Sorensen's ideas.
;*
;* Rev 1.1 - 15-Jul-97 : Added define check
;* Rev 1.2 - 18-Jul-97 : Added revision check macro
;* Rev 1.3 - 19-Jul-97 : Modified for RGBASM V1.05
;* Rev 1.4 - 27-Jul-97 : Modified for new subroutine prefixes
;* Rev 1.5 - 15-Aug-97 : Added _HRAM, PAD, CART defines
;*                     :  and Nintendo Logo
;* Rev 1.6 - 30-Nov-97 : Added rDIV, rTIMA, rTMA, & rTAC
;* Rev 1.7 - 31-Jan-98 : Added _SCRN0, _SCRN1
;* Rev 1.8 - 15-Feb-98 : Added rSB, rSC
;* Rev 1.9 - 16-Feb-98 : Converted I/O registers to 0xFFXX format
;* Rev 2.0 -           : Added GBC registers
;* Rev 2.1 -           : Added MBC5 & cart RAM enable/disable defines
;* Rev 2.2 -           : Fixed NR42,NR43, & NR44 .equates
;* Rev 2.3 -           : Fixed incorrect _HRAM .equate
;* Rev 2.4 - 27-Apr-13 : Added some cart defines (AntonioND)
;* Rev 2.5 - 03-May-15 : Fixed format (AntonioND)
;* Rev 2.6 - 09-Apr-16 : Added GBC OAM and cart defines (AntonioND)
;* Rev 2.7 - 19-Jan-19 : Added rPCMXX (ISSOtm)
;* Rev 2.8 - 03-Feb-19 : Added audio registers flags (Álvaro Cuesta)
;* Rev 2.9 - 28-Feb-20 : Added utility rP1 constants
;* Rev 3.0 - 27-Aug-20 : Register ordering, byte-based sizes, OAM additions, general cleanup (Blitter Object)
;* Rev 4.0 - 03-May-21 : Updated to use RGBDS 0.5.0 syntax, changed IEF_LCDC to IEF_STAT (Eievui)
;* Rev 4.1 - 16-Aug-21 : Added more flags, bit number defines, and offset constants for OAM and window positions (rondnelson99)
;* Rev 4.2 - 04-Sep-21 : Added CH3- and CH4-specific audio registers flags (ISSOtm)
;* Rev 4.3 - 07-Nov-21 : Deprecate VRAM address constants (Eievui)
;* Rev 4.4 - 11-Jan-22 : Deprecate VRAM CART_SRAM_2KB constant (avivace)
;* Rev 4.5 - 03-Mar-22 : Added bit number definitions for OCPS, BCPS and LCDC (sukus)
;* Rev 4.6 - 15-Jun-22 : Added MBC3 registers and special values
;* Rev 4.7.0 - 27-Jun-22 : Added alternate names for some constants
;* Rev 4.7.1 - 05-Jul-22 : Added RPB_LED_ON constant
;* Rev 4.8.0 - 25-Oct-22 : Changed background addressing constants (zlago)


;***************************************************************************
;*
;* General memory region constants
;*
;***************************************************************************


_VRAM                           .equ    0x8000 ;; 0x8000->0x9FFF
_SCRN0                          .equ    0x9800 ;; 0x9800->0x9BFF
_SCRN1                          .equ    0x9C00 ;; 0x9C00->0x9FFF
_SRAM                           .equ    0xA000 ;; 0xA000->0xBFFF
_RAM                            .equ    0xC000 ;; 0xC000->0xCFFF / 0xC000->0xDFFF
_RAMBANK                        .equ    0xD000 ;; 0xD000->0xDFFF
_OAMRAM                         .equ    0xFE00 ;; 0xFE00->0xFE9F
_IO                             .equ    0xFF00 ;; 0xFF00->0xFF7F,0xFFFF
_AUD3WAVERAM                    .equ    0xFF30 ;; 0xFF30->0xFF3F
_HRAM                           .equ    0xFF80 ;; 0xFF80->0xFFFE


;;#**************************************************************************
;;#
;;# MBC registers
;;#
;;#**************************************************************************

;; *** Common ***

;; --
;; -- RAMG (0x0000-0x1FFF)
;; -- Controls whether access to SRAM (and the MBC3 RTC registers) is allowed (W)
;; --
rRAMG                           .equ    0x0000

CART_SRAM_ENABLE                .equ    0x0A
CART_SRAM_DISABLE               .equ    0x00


;; --
;; -- ROMB0 (0x2000-0x3FFF)
;; -- Selects which ROM bank is mapped to the ROMX space (0x4000-0x7FFF) (W)
;; --
;; -- The range of accepted values, as well as the behavior of writing 0x00,
;; -- varies depending on the MBC.
;; --
rROMB0                          .equ    0x2000

;; --
;; -- RAMB (0x4000-0x5FFF)
;; -- Selects which SRAM bank is mapped to the SRAM space (0xA000-0xBFFF) (W)
;; --
;; -- The range of accepted values varies depending on the cartridge configuration.
;; --
rRAMB                           .equ    0x4000


;; *** MBC3-specific registers ***

;; Write one of these to rRAMG to map the corresponding RTC register to all SRAM space
RTC_S                           .equ    0x08    ;; Seconds  (0-59)
RTC_M                           .equ    0x09    ;; Minutes  (0-59)
RTC_H                           .equ    0x0A    ;; Hours    (0-23)
RTC_DL                          .equ    0x0B    ;; Lower 8 bits of Day Counter (0x00-0xFF)
RTC_DH                          .equ    0x0C    ;; Bit 7 - Day Counter Carry Bit (1=Counter Overflow)
                                                ;; Bit 6 - Halt (0=Active, 1=Stop Timer)
                                                ;; Bit 0 - Most significant bit of Day Counter (Bit 8)


;; --
;; -- RTCLATCH (0x6000-0x7FFF)
;; -- Write 0x00 then 0x01 to latch the current time into the RTC registers (W)
;; --
rRTCLATCH                       .equ    0x6000


;; *** MBC5-specific register ***

;; --
;; -- ROMB1 (0x3000-0x3FFF)
;; -- A 9th bit that "extends" ROMB0 if more than 256 banks are present (W)
;; --
;; -- Also note that rROMB0 thus only spans 0x2000-0x2FFF.
;; --
rROMB1                          .equ    0x3000


;; Bit 3 of RAMB enables the rumble motor (if any)
CART_RUMBLE_ON                  .equ    1 << 3


;;#**************************************************************************
;;#
;;# Memory-mapped registers
;;#
;;#**************************************************************************

;; --
;; -- P1 (0xFF00)
;; -- Register for reading joy pad info. (R/W)
;; --
rP1                             .equ    0xFF00

P1F_5                           .equ    0x20 ;; %0010 0000 ;; P15 out port, set to 0 to get buttons
P1F_4                           .equ    0x10 ;; %0001 0000 ;; P14 out port, set to 0 to get dpad
P1F_3                           .equ    0x08 ;; %0000 1000 ;; P13 in port
P1F_2                           .equ    0x04 ;; %0000 0100 ;; P12 in port
P1F_1                           .equ    0x02 ;; %0000 0010 ;; P11 in port
P1F_0                           .equ    0x01 ;; %0000 0001 ;; P10 in port

P1F_GET_DPAD                    .equ    P1F_5
P1F_GET_BTN                     .equ    P1F_4
P1F_GET_NONE                    .equ    P1F_4 | P1F_5


;; --
;; -- SB (0xFF01)
;; -- Serial Transfer Data (R/W)
;; --
rSB                             .equ    0xFF01


;; --
;; -- SC (0xFF02)
;; -- Serial I/O Control (R/W)
;; --
rSC                             .equ    0xFF02

SCF_START                       .equ    0x80 ;; %10000000 ;; Transfer Start Flag (1=Transfer in progress, or requested)
SCF_SPEED                       .equ    0x02 ;; %00000010 ;; Clock Speed (0=Normal, 1=Fast) ** CGB Mode Only **
SCF_SOURCE                      .equ    0x01 ;; %00000001 ;; Shift Clock (0=External Clock, 1=Internal Clock)

SCB_START                       .equ    7
SCB_SPEED                       .equ    1
SCB_SOURCE                      .equ    0

;; --
;; -- DIV (0xFF04)
;; -- Divider register (R/W)
;; --
rDIV                            .equ    0xFF04


;; --
;; -- TIMA (0xFF05)
;; -- Timer counter (R/W)
;; --
rTIMA                           .equ    0xFF05


;; --
;; -- TMA (0xFF06)
;; -- Timer modulo (R/W)
;; --
rTMA                            .equ    0xFF06


;; --
;; -- TAC (0xFF07)
;; -- Timer control (R/W)
;; --
rTAC                            .equ    0xFF07

TACF_START                      .equ    0x04 ;; %00000100
TACF_STOP                       .equ    0x00 ;; %00000000
TACF_4KHZ                       .equ    0x00 ;; %00000000
TACF_16KHZ                      .equ    0x03 ;; %00000011
TACF_65KHZ                      .equ    0x02 ;; %00000010
TACF_262KHZ                     .equ    0x01 ;; %00000001

TACB_START                      .equ    2


;; --
;; -- IF (0xFF0F)
;; -- Interrupt Flag (R/W)
;; --
rIF                             .equ    0xFF0F


;; --
;; -- AUD1SWEEP/NR10 (0xFF10)
;; -- Sweep register (R/W)
;; --
;; -- Bit 6-4 - Sweep Time
;; -- Bit 3   - Sweep Increase/Decrease
;; --           0: Addition    (frequency increases?)
;; --           1: Subtraction (frequency increases?)
;; -- Bit 2-0 - Number of sweep shift (# 0-7)
;; -- Sweep Time: (n*7.8ms)
;; --
rNR10                           .equ    0xFF10
rAUD1SWEEP                      .equ    rNR10

AUD1SWEEP_UP                    .equ    0x00 ;; %00000000
AUD1SWEEP_DOWN                  .equ    0x08 ;; %00001000


;; --
;; -- AUD1LEN/NR11 (0xFF11)
;; -- Sound length/Wave pattern duty (R/W)
;; --
;; -- Bit 7-6 - Wave Pattern Duty (00:12.5% 01:25% 10:50% 11:75%)
;; -- Bit 5-0 - Sound length data (# 0-63)
;; --
rNR11                           .equ    0xFF11
rAUD1LEN                        .equ    rNR11


;; --
;; -- AUD1ENV/NR12 (0xFF12)
;; -- Envelope (R/W)
;; --
;; -- Bit 7-4 - Initial value of envelope
;; -- Bit 3   - Envelope UP/DOWN
;; --           0: Decrease
;; --           1: Range of increase
;; -- Bit 2-0 - Number of envelope sweep (# 0-7)
;; --
rNR12                           .equ    0xFF12
rAUD1ENV                        .equ    rNR12


;; --
;; -- AUD1LOW/NR13 (0xFF13)
;; -- Frequency low byte (W)
;; --
rNR13                           .equ    0xFF13
rAUD1LOW                        .equ    rNR13


;; --
;; -- AUD1HIGH/NR14 (0xFF14)
;; -- Frequency high byte (W)
;; --
;; -- Bit 7   - Initial (when set, sound restarts)
;; -- Bit 6   - Counter/consecutive selection
;; -- Bit 2-0 - Frequency`s higher 3 bits
;; --
rNR14                           .equ    0xFF14
rAUD1HIGH                       .equ    rNR14


;; --
;; -- AUD2LEN/NR21 (0xFF16)
;; -- Sound Length;; Wave Pattern Duty (R/W)
;; --
;; -- see AUD1LEN for info
;; --
rNR21                           .equ    0xFF16
rAUD2LEN                        .equ    rNR21


;; --
;; -- AUD2ENV/NR22 (0xFF17)
;; -- Envelope (R/W)
;; --
;; -- see AUD1ENV for info
;; --
rNR22                           .equ    0xFF17
rAUD2ENV                        .equ    rNR22


;; --
;; -- AUD2LOW/NR23 (0xFF18)
;; -- Frequency low byte (W)
;; --
rNR23                           .equ    0xFF18
rAUD2LOW                        .equ    rNR23


;; --
;; -- AUD2HIGH/NR24 (0xFF19)
;; -- Frequency high byte (W)
;; --
;; -- see AUD1HIGH for info
;; --
rNR24                           .equ    0xFF19
rAUD2HIGH                       .equ    rNR24


;; --
;; -- AUD3ENA/NR30 (0xFF1A)
;; -- Sound on/off (R/W)
;; --
;; -- Bit 7   - Sound ON/OFF (1=ON,0=OFF)
;; --
rNR30                           .equ    0xFF1A
rAUD3ENA                        .equ    rNR30

AUD3ENA_OFF                     .equ    0x00 ;; %00000000
AUD3ENA_ON                      .equ    0x80 ;; %10000000


;; --
;; -- AUD3LEN/NR31 (0xFF1B)
;; -- Sound length (R/W)
;; --
;; -- Bit 7-0 - Sound length
;; --
rNR31                           .equ    0xFF1B
rAUD3LEN                        .equ    rNR31


;; --
;; -- AUD3LEVEL/NR32 (0xFF1C)
;; -- Select output level
;; --
;; -- Bit 6-5 - Select output level
;; --           00: 0/1 (mute)
;; --           01: 1/1
;; --           10: 1/2
;; --           11: 1/4
;; --
rNR32                           .equ    0xFF1C
rAUD3LEVEL                      .equ    rNR32

AUD3LEVEL_MUTE                  .equ    0x00 ;; %0000 0000
AUD3LEVEL_100                   .equ    0x20 ;; %0010 0000
AUD3LEVEL_50                    .equ    0x40 ;; %0100 0000
AUD3LEVEL_25                    .equ    0x60 ;; %0110 0000


;; --
;; -- AUD3LOW/NR33 (0xFF1D)
;; -- Frequency low byte (W)
;; --
;; -- see AUD1LOW for info
;; --
rNR33                           .equ    0xFF1D
rAUD3LOW                        .equ    rNR33


;; --
;; -- AUD3HIGH/NR34 (0xFF1E)
;; -- Frequency high byte (W)
;; --
;; -- see AUD1HIGH for info
;; --
rNR34                           .equ    0xFF1E
rAUD3HIGH                       .equ    rNR34


;; --
;; -- AUD4LEN/NR41 (0xFF20)
;; -- Sound length (R/W)
;; --
;; -- Bit 5-0 - Sound length data (# 0-63)
;; --
rNR41                           .equ    0xFF20
rAUD4LEN                        .equ    rNR41


;; --
;; -- AUD4ENV/NR42 (0xFF21)
;; -- Envelope (R/W)
;; --
;; -- see AUD1ENV for info
;; --
rNR42                           .equ    0xFF21
rAUD4ENV                        .equ    rNR42


;; --
;; -- AUD4POLY/NR43 (0xFF22)
;; -- Polynomial counter (R/W)
;; --
;; -- Bit 7-4 - Selection of the shift clock frequency of the (scf)
;; --           polynomial counter (0000-1101)
;; --           freq=drf*1/2^scf (not sure)
;; -- Bit 3 -   Selection of the polynomial counter`s step
;; --           0: 15 steps
;; --           1: 7 steps
;; -- Bit 2-0 - Selection of the dividing ratio of frequencies (drf)
;; --           000: f/4   001: f/8   010: f/16  011: f/24
;; --           100: f/32  101: f/40  110: f/48  111: f/56  (f=4.194304 Mhz)
;; --
rNR43                           .equ    0xFF22
rAUD4POLY                       .equ    rNR43

AUD4POLY_15STEP                 .equ    0x00 ;; %00000000
AUD4POLY_7STEP                  .equ    0x08 ;; %00001000


;; --
;; -- AUD4GO/NR44 (0xFF23)
;; --
;; -- Bit 7 -   Initial (when set, sound restarts)
;; -- Bit 6 -   Counter/consecutive selection
;; --
rNR44                           .equ    0xFF23
rAUD4GO                         .equ    rNR44





;; --
;; -- AUDVOL/NR50 (0xFF24)
;; -- Channel control / ON-OFF / Volume (R/W)
;; --
;; -- Bit 7   - Vin->SO2 ON/OFF (left)
;; -- Bit 6-4 - SO2 output level (left speaker) (# 0-7)
;; -- Bit 3   - Vin->SO1 ON/OFF (right)
;; -- Bit 2-0 - SO1 output level (right speaker) (# 0-7)
;; --
rNR50                           .equ    0xFF24
rAUDVOL                         .equ    rNR50

AUDVOL_VIN_LEFT                 .equ    0x80 ;; %10000000 ;; SO2
AUDVOL_VIN_RIGHT                .equ    0x08 ;; %00001000 ;; SO1


;; --
;; -- AUDTERM/NR51 (0xFF25)
;; -- Selection of Sound output terminal (R/W)
;; --
;; -- Bit 7   - Output channel 4 to SO2 terminal (left)
;; -- Bit 6   - Output channel 3 to SO2 terminal (left)
;; -- Bit 5   - Output channel 2 to SO2 terminal (left)
;; -- Bit 4   - Output channel 1 to SO2 terminal (left)
;; -- Bit 3   - Output channel 4 to SO1 terminal (right)
;; -- Bit 2   - Output channel 3 to SO1 terminal (right)
;; -- Bit 1   - Output channel 2 to SO1 terminal (right)
;; -- Bit 0   - Output channel 1 to SO1 terminal (right)
;; --
rNR51                           .equ    0xFF25
rAUDTERM                        .equ    rNR51

;; SO2
AUDTERM_4_LEFT                  .equ    0x80 ;; %1000 0000
AUDTERM_3_LEFT                  .equ    0x40 ;; %0100 0000
AUDTERM_2_LEFT                  .equ    0x20 ;; %0010 0000
AUDTERM_1_LEFT                  .equ    0x10 ;; %0001 0000
;; SO1
AUDTERM_4_RIGHT                 .equ    0x08 ;; %0000 1000
AUDTERM_3_RIGHT                 .equ    0x04 ;; %0000 0100
AUDTERM_2_RIGHT                 .equ    0x02 ;; %0000 0010
AUDTERM_1_RIGHT                 .equ    0x01 ;; %0000 0001


;; --
;; -- AUDENA/NR52 (0xFF26)
;; -- Sound on/off (R/W)
;; --
;; -- Bit 7   - All sound on/off (sets all audio regs to 0!)
;; -- Bit 3   - Sound 4 ON flag (read only)
;; -- Bit 2   - Sound 3 ON flag (read only)
;; -- Bit 1   - Sound 2 ON flag (read only)
;; -- Bit 0   - Sound 1 ON flag (read only)
;; --
rNR52                           .equ    0xFF26
rAUDENA                         .equ    rNR52

AUDENA_ON                       .equ    0x80 ;; %10000000
AUDENA_OFF                      .equ    0x00 ;; %00000000  ;; sets all audio regs to 0!


;; --
;; -- LCDC (0xFF40)
;; -- LCD Control (R/W)
;; --
rLCDC                           .equ    0xFF40

LCDCF_OFF                       .equ    0x00 ;; %0000 0000 ;; LCD Control Operation
LCDCF_ON                        .equ    0x80 ;; %1000 0000 ;; LCD Control Operation
LCDCF_WIN9800                   .equ    0x00 ;; %0000 0000 ;; Window Tile Map Display Select
LCDCF_WIN9C00                   .equ    0x40 ;; %0100 0000 ;; Window Tile Map Display Select
LCDCF_WINOFF                    .equ    0x00 ;; %0000 0000 ;; Window Display
LCDCF_WINON                     .equ    0x20 ;; %0010 0000 ;; Window Display
LCDCF_BLK21                     .equ    0x00 ;; %0000 0000 ;; BG & Window Tile Data Select
LCDCF_BLK01                     .equ    0x10 ;; %0001 0000 ;; BG & Window Tile Data Select
LCDCF_BG9800                    .equ    0x00 ;; %0000 0000 ;; BG Tile Map Display Select
LCDCF_BG9C00                    .equ    0x08 ;; %0000 1000 ;; BG Tile Map Display Select
LCDCF_OBJ8                      .equ    0x00 ;; %0000 0000 ;; OBJ Construction
LCDCF_OBJ16                     .equ    0x04 ;; %0000 0100 ;; OBJ Construction
LCDCF_OBJOFF                    .equ    0x00 ;; %0000 0000 ;; OBJ Display
LCDCF_OBJON                     .equ    0x02 ;; %0000 0010 ;; OBJ Display
LCDCF_BGOFF                     .equ    0x00 ;; %0000 0000 ;; BG Display
LCDCF_BGON                      .equ    0x01 ;; %0000 0001 ;; BG Display

LCDCB_ON                        .equ    7 ;; LCD Control Operation
LCDCB_WIN9C00                   .equ    6 ;; Window Tile Map Display Select
LCDCB_WINON                     .equ    5 ;; Window Display
LCDCB_BLKS                      .equ    4 ;; BG & Window Tile Data Select
LCDCB_BG9C00                    .equ    3 ;; BG Tile Map Display Select
LCDCB_OBJ16                     .equ    2 ;; OBJ Construction
LCDCB_OBJON                     .equ    1 ;; OBJ Display
LCDCB_BGON                      .equ    0 ;; BG Display
;; "Window Character Data Select" follows BG


;; --
;; -- STAT (0xFF41)
;; -- LCDC Status   (R/W)
;; --
rSTAT                           .equ    0xFF41

STATF_LYC                       .equ    0x40 ;; %0100 0000 ;; LYC=LY Coincidence (Selectable)
STATF_MODE10                    .equ    0x20 ;; %0010 0000 ;; Mode 10
STATF_MODE01                    .equ    0x10 ;; %0001 0000 ;; Mode 01 (V-Blank)
STATF_MODE00                    .equ    0x08 ;; %0000 1000 ;; Mode 00 (H-Blank)
STATF_LYCF                      .equ    0x04 ;; %0000 0100 ;; Coincidence Flag
STATF_HBL                       .equ    0x00 ;; %0000 0000 ;; H-Blank
STATF_VBL                       .equ    0x01 ;; %0000 0001 ;; V-Blank
STATF_OAM                       .equ    0x02 ;; %0000 0010 ;; OAM-RAM is used by system
STATF_LCD                       .equ    0x03 ;; %0000 0011 ;; Both OAM and VRAM used by system
STATF_BUSY                      .equ    0x02 ;; %0000 0010 ;; When set, VRAM access is unsafe

STATB_LYC                       .equ    6
STATB_MODE10                    .equ    5
STATB_MODE01                    .equ    4
STATB_MODE00                    .equ    3
STATB_LYCF                      .equ    2
STATB_BUSY                      .equ    1

;; --
;; -- SCY (0xFF42)
;; -- Scroll Y (R/W)
;; --
rSCY                            .equ    0xFF42


;; --
;; -- SCX (0xFF43)
;; -- Scroll X (R/W)
;; --
rSCX                            .equ    0xFF43


;; --
;; -- LY (0xFF44)
;; -- LCDC Y-Coordinate (R)
;; --
;; -- Values range from 0->153. 144->153 is the VBlank period.
;; --
rLY                             .equ    0xFF44


;; --
;; -- LYC (0xFF45)
;; -- LY Compare (R/W)
;; --
;; -- When LY==LYC, STATF_LYCF will be set in STAT
;; --
rLYC                            .equ    0xFF45


;; --
;; -- DMA (0xFF46)
;; -- DMA Transfer and Start Address (W)
;; --
rDMA                            .equ    0xFF46


;; --
;; -- BGP (0xFF47)
;; -- BG Palette Data (W)
;; --
;; -- Bit 7-6 - Intensity for %11
;; -- Bit 5-4 - Intensity for %10
;; -- Bit 3-2 - Intensity for %01
;; -- Bit 1-0 - Intensity for %00
;; --
rBGP                            .equ    0xFF47


;; --
;; -- OBP0 (0xFF48)
;; -- Object Palette 0 Data (W)
;; --
;; -- See BGP for info
;; --
rOBP0                           .equ    0xFF48


;; --
;; -- OBP1 (0xFF49)
;; -- Object Palette 1 Data (W)
;; --
;; -- See BGP for info
;; --
rOBP1                           .equ    0xFF49


;; --
;; -- WY (0xFF4A)
;; -- Window Y Position (R/W)
;; --
;; -- 0 <= WY <= 143
;; -- When WY = 0, the window is displayed from the top edge of the LCD screen.
;; --
rWY                             .equ    0xFF4A


;; --
;; -- WX (0xFF4B)
;; -- Window X Position (R/W)
;; --
;; -- 7 <= WX <= 166
;; -- When WX = 7, the window is displayed from the left edge of the LCD screen.
;; -- Values of 0-6 and 166 are unreliable due to hardware bugs.
;; --
rWX                             .equ    0xFF4B


;; -- This unknown register is at the tail end of the LCD register space. 
;; -- It may possibly lockdown advanced color hardware features from being
;; -- accessed by a game in DMG mode. This could be tested. The CGB boot ROM
;; -- sets this to $80 if a CGB game is inserted, or $04 if a DMG game is
;; -- inserted.
rLCDMODE                        .equ    0xFF4C

WX_OFS                          .equ    7 ;; add this to a screen position to get a WX position


;; --
;; -- SPEED (0xFF4D)
;; -- Select CPU Speed (R/W)
;; --
rKEY1                           .equ    0xFF4D
rSPD                            .equ    rKEY1

KEY1F_DBLSPEED                  .equ    0x80 ;; %10000000 ;; 0=Normal Speed, 1=Double Speed (R)
KEY1F_PREPARE                   .equ    0x01 ;; %00000001 ;; 0=No, 1=Prepare (R/W)


;; --
;; -- VBK (0xFF4F)
;; -- Select Video RAM Bank (R/W)
;; --
;; -- Bit 0 - Bank Specification (0: Specify Bank 0;; 1: Specify Bank 1)
;; --
rVBK                            .equ    0xFF4F

;; -- Assigning this address to value 0x11 disables boot ROM. This is done by
;; -- GBC bios during boot process. This unmaps GBC bios from 0x00 - 0xFF and
;; -- instead maps start of ROM cartridge data to this memory space
rBLCK                           .equ    0xFF50

;; --
;; -- HDMA1 (0xFF51)
;; -- High byte for Horizontal Blanking/General Purpose DMA source address (W)
;; -- CGB Mode Only
;; --
rHDMA1                          .equ    0xFF51


;; --
;; -- HDMA2 (0xFF52)
;; -- Low byte for Horizontal Blanking/General Purpose DMA source address (W)
;; -- CGB Mode Only
;; --
rHDMA2                          .equ    0xFF52


;; --
;; -- HDMA3 (0xFF53)
;; -- High byte for Horizontal Blanking/General Purpose DMA destination address (W)
;; -- CGB Mode Only
;; --
rHDMA3                          .equ    0xFF53


;; --
;; -- HDMA4 (0xFF54)
;; -- Low byte for Horizontal Blanking/General Purpose DMA destination address (W)
;; -- CGB Mode Only
;; --
rHDMA4                          .equ    0xFF54


;; --
;; -- HDMA5 (0xFF55)
;; -- Transfer length (in tiles minus 1)/mode/start for Horizontal Blanking, General Purpose DMA (R/W)
;; -- CGB Mode Only
;; --
rHDMA5                          .equ    0xFF55

HDMA5F_MODE_GP                  .equ    0x00 ;; %00000000 ;; General Purpose DMA (W)
HDMA5F_MODE_HBL                 .equ    0x80 ;; %10000000 ;; HBlank DMA (W)
HDMA5B_MODE                     .equ    7                 ;; DMA mode select (W)

;; -- Once DMA has started, use HDMA5F_BUSY to check when the transfer is complete
HDMA5F_BUSY                     .equ    0x80 ;; %10000000 ;; 0=Busy (DMA still in progress), 1=Transfer complete (R)


;; --
;; -- RP (0xFF56)
;; -- Infrared Communications Port (R/W)
;; -- CGB Mode Only
;; --
rRP                             .equ    0xFF56

RPF_ENREAD                      .equ    0xC0 ;; %11000000
RPF_DATAIN                      .equ    0x02 ;; %00000010 ;; 0=Receiving IR Signal, 1=Normal
RPF_WRITE_HI                    .equ    0x01 ;; %00000001
RPF_WRITE_LO                    .equ    0x00 ;; %00000000

RPB_LED_ON                      .equ    0
RPB_DATAIN                      .equ    1


;; --
;; -- BCPS/BGPI (0xFF68)
;; -- Background Color Palette Specification (aka Background Palette Index) (R/W)
;; --
rBCPS                           .equ    0xFF68
rBGPI                           .equ    rBCPS

BCPSF_AUTOINC                   .equ    0x80 ;; %10000000 ;; Auto Increment (0=Disabled, 1=Increment after Writing)
BCPSB_AUTOINC                   .equ    7
BGPIF_AUTOINC                   .equ    BCPSF_AUTOINC
BGPIB_AUTOINC                   .equ    BCPSB_AUTOINC


;; --
;; -- BCPD/BGPD (0xFF69)
;; -- Background Color Palette Data (aka Background Palette Data) (R/W)
;; --
rBCPD                           .equ    0xFF69
rBGPD                           .equ    rBCPD


;; --
;; -- OCPS/OBPI (0xFF6A)
;; -- Object Color Palette Specification (aka Object Background Palette Index) (R/W)
;; --
rOCPS                           .equ    0xFF6A
rOBPI                           .equ    rOCPS

OCPSF_AUTOINC                   .equ    0x80 ;; %10000000 ;; Auto Increment (0=Disabled, 1=Increment after Writing)
OCPSB_AUTOINC                   .equ    7
OBPIF_AUTOINC                   .equ    OCPSF_AUTOINC
OBPIB_AUTOINC                   .equ    OCPSB_AUTOINC


;; --
;; -- OCPD/OBPD (0xFF6B)
;; -- Object Color Palette Data (aka Object Background Palette Data) (R/W)
;; --
rOCPD                           .equ    0xFF6B
rOBPD                           .equ    rOCPD


;; --
;; -- SMBK/SVBK (0xFF70)
;; -- Select Main RAM Bank (R/W)
;; --
;; -- Bit 2-0 - Bank Specification (0,1: Specify Bank 1;; 2-7: Specify Banks 2-7)
;; --
rSVBK                           .equ    0xFF70
rSMBK                           .equ    rSVBK


;; --
;; -- PCM12 (0xFF76)
;; -- Sound channel 1&2 PCM amplitude (R)
;; --
;; -- Bit 7-4 - Copy of sound channel 2`s PCM amplitude
;; -- Bit 3-0 - Copy of sound channel 1`s PCM amplitude
;; --
rPCM12                          .equ    0xFF76


;; --
;; -- PCM34 (0xFF77)
;; -- Sound channel 3&4 PCM amplitude (R)
;; --
;; -- Bit 7-4 - Copy of sound channel 4`s PCM amplitude
;; -- Bit 3-0 - Copy of sound channel 3`s PCM amplitude
;; --
rPCM34                          .equ    0xFF77


;; --
;; -- IE (0xFFFF)
;; -- Interrupt Enable (R/W)
;; --
rIE                             .equ    0xFFFF

IEF_HILO                        .equ    0x10 ;; %0001 0000 ;; Transition from High to Low of Pin number P10-P13
IEF_SERIAL                      .equ    0x08 ;; %0000 1000 ;; Serial I/O transfer end
IEF_TIMER                       .equ    0x04 ;; %0000 0100 ;; Timer Overflow
IEF_STAT                        .equ    0x02 ;; %0000 0010 ;; STAT
IEF_VBLANK                      .equ    0x01 ;; %0000 0001 ;; V-Blank

IEB_HILO                        .equ    4
IEB_SERIAL                      .equ    3
IEB_TIMER                       .equ    2
IEB_STAT                        .equ    1
IEB_VBLANK                      .equ    0


;;#**************************************************************************
;;#
;;# Flags common to multiple sound channels
;;#
;;#**************************************************************************

;; --
;; -- Square wave duty cycle
;; --
;; -- Can be used with AUD1LEN and AUD2LEN
;; -- See AUD1LEN for more info
;; --
AUDLEN_DUTY_12_5                .equ    0x00 ;; %0000 0000 ;; 12.5%
AUDLEN_DUTY_25                  .equ    0x40 ;; %0100 0000 ;; 25%
AUDLEN_DUTY_50                  .equ    0x80 ;; %1000 0000 ;; 50%
AUDLEN_DUTY_75                  .equ    0xC0 ;; %1100 0000 ;; 75%


;; --
;; -- Audio envelope flags
;; --
;; -- Can be used with AUD1ENV, AUD2ENV, AUD4ENV
;; -- See AUD1ENV for more info
;; --
AUDENV_UP                       .equ    0x08 ;; %0000 1000
AUDENV_DOWN                     .equ    0x00 ;; %0000 0000


;; --
;; -- Audio trigger flags
;; --
;; -- Can be used with AUD1HIGH, AUD2HIGH, AUD3HIGH
;; -- See AUD1HIGH for more info
;; --
AUDHIGH_RESTART                 .equ    0x80 ;; %1000 0000
AUDHIGH_LENGTH_ON               .equ    0x40 ;; %0100 0000
AUDHIGH_LENGTH_OFF              .equ    0x00 ;; %0000 0000


;;#**************************************************************************
;;#
;;# CPU values on bootup (a=type, b=qualifier)
;;#
;;#**************************************************************************

BOOTUP_A_DMG                    .equ    0x01 ;; Dot Matrix Game
BOOTUP_A_CGB                    .equ    0x11 ;; Color GameBoy
BOOTUP_A_MGB                    .equ    0xFF ;; Mini GameBoy (Pocket GameBoy)

;; if a=BOOTUP_A_CGB, bit 0 in b can be checked to determine if real CGB or
;; other system running in GBC mode
BOOTUP_B_CGB                    .equ    0x00 ;; %0000 0000
BOOTUP_B_AGB                    .equ    0x01 ;; %0000 0001   ;; GBA, GBA SP, Game Boy Player, or New GBA SP


;;#**************************************************************************
;;#
;;# Header
;;#
;;#**************************************************************************

;;#
;;# Nintendo scrolling logo
;;# (Code won`t work on a real GameBoy)
;;# (if next lines are altered.)
.macro NINTENDO_LOGO
    DB  0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D
    DB  0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99
    DB  0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E
.endm

;; 0x0143 Color GameBoy compatibility code
CART_COMPATIBLE_DMG             .equ    0x00
CART_COMPATIBLE_DMG_GBC         .equ    0x80
CART_COMPATIBLE_GBC             .equ    0xC0

;; 0x0146 GameBoy/Super GameBoy indicator
CART_INDICATOR_GB               .equ    0x00
CART_INDICATOR_SGB              .equ    0x03

;; 0x0147 Cartridge type
CART_ROM                        .equ    0x00
CART_ROM_MBC1                   .equ    0x01
CART_ROM_MBC1_RAM               .equ    0x02
CART_ROM_MBC1_RAM_BAT           .equ    0x03
CART_ROM_MBC2                   .equ    0x05
CART_ROM_MBC2_BAT               .equ    0x06
CART_ROM_RAM                    .equ    0x08
CART_ROM_RAM_BAT                .equ    0x09
CART_ROM_MMM01                  .equ    0x0B
CART_ROM_MMM01_RAM              .equ    0x0C
CART_ROM_MMM01_RAM_BAT          .equ    0x0D
CART_ROM_MBC3_BAT_RTC           .equ    0x0F
CART_ROM_MBC3_RAM_BAT_RTC       .equ    0x10
CART_ROM_MBC3                   .equ    0x11
CART_ROM_MBC3_RAM               .equ    0x12
CART_ROM_MBC3_RAM_BAT           .equ    0x13
CART_ROM_MBC5                   .equ    0x19
CART_ROM_MBC5_BAT               .equ    0x1A
CART_ROM_MBC5_RAM_BAT           .equ    0x1B
CART_ROM_MBC5_RUMBLE            .equ    0x1C
CART_ROM_MBC5_RAM_RUMBLE        .equ    0x1D
CART_ROM_MBC5_RAM_BAT_RUMBLE    .equ    0x1E
CART_ROM_MBC7_RAM_BAT_GYRO      .equ    0x22
CART_ROM_POCKET_CAMERA          .equ    0xFC
CART_ROM_BANDAI_TAMA5           .equ    0xFD
CART_ROM_HUDSON_HUC3            .equ    0xFE
CART_ROM_HUDSON_HUC1            .equ    0xFF

;; 0x0148 ROM size
;; these are kilobytes
CART_ROM_32KB                   .equ    0x00 ;; 2 banks
CART_ROM_64KB                   .equ    0x01 ;; 4 banks
CART_ROM_128KB                  .equ    0x02 ;; 8 banks
CART_ROM_256KB                  .equ    0x03 ;; 16 banks
CART_ROM_512KB                  .equ    0x04 ;; 32 banks
CART_ROM_1024KB                 .equ    0x05 ;; 64 banks
CART_ROM_2048KB                 .equ    0x06 ;; 128 banks
CART_ROM_4096KB                 .equ    0x07 ;; 256 banks
CART_ROM_8192KB                 .equ    0x08 ;; 512 banks
CART_ROM_1152KB                 .equ    0x52 ;; 72 banks
CART_ROM_1280KB                 .equ    0x53 ;; 80 banks
CART_ROM_1536KB                 .equ    0x54 ;; 96 banks

;; 0x0149 SRAM size
;; these are kilobytes
CART_SRAM_NONE                  .equ    0
CART_SRAM_8KB                   .equ    2 ;; 1 bank
CART_SRAM_32KB                  .equ    3 ;; 4 banks
CART_SRAM_128KB                 .equ    4 ;; 16 banks

;; 0x014A Destination code
CART_DEST_JAPANESE              .equ    0x00
CART_DEST_NON_JAPANESE          .equ    0x01


;;#**************************************************************************
;;#
;;# Keypad related
;;#
;;#**************************************************************************

PADF_DOWN                       .equ    0x80
PADF_UP                         .equ    0x40
PADF_LEFT                       .equ    0x20
PADF_RIGHT                      .equ    0x10
PADF_START                      .equ    0x08
PADF_SELECT                     .equ    0x04
PADF_B                          .equ    0x02
PADF_A                          .equ    0x01

PADB_DOWN                       .equ    0x7
PADB_UP                         .equ    0x6
PADB_LEFT                       .equ    0x5
PADB_RIGHT                      .equ    0x4
PADB_START                      .equ    0x3
PADB_SELECT                     .equ    0x2
PADB_B                          .equ    0x1
PADB_A                          .equ    0x0


;;#**************************************************************************
;;#
;;# Screen related
;;#
;;#**************************************************************************

SCRN_X                          .equ    160 ;; Width of screen in pixels
SCRN_Y                          .equ    144 ;; Height of screen in pixels. Also corresponds to the value in LY at the beginning of VBlank.
SCRN_X_B                        .equ    20  ;; Width of screen in bytes
SCRN_Y_B                        .equ    18  ;; Height of screen in bytes

SCRN_VX                         .equ    256 ;; Virtual width of screen in pixels
SCRN_VY                         .equ    256 ;; Virtual height of screen in pixels
SCRN_VX_B                       .equ    32  ;; Virtual width of screen in bytes
SCRN_VY_B                       .equ    32  ;; Virtual height of screen in bytes


;;#**************************************************************************
;;#
;;# OAM related
;;#
;;#**************************************************************************

;; OAM attributes
;; each entry in OAM RAM is 4 bytes (sizeof_OAM_ATTRS)
;; !! Not sure what this is and why its here -> RSRESET
; DEF OAMA_Y                 RB  1   ;; y pos plus 16
; DEF OAMA_X                 RB  1   ;; x pos plus 8
; DEF OAMA_TILEID            RB  1   ;; tile id
; DEF OAMA_FLAGS             RB  1   ;; flags (see below)
; DEF sizeof_OAM_ATTRS       RB  0

OAM_Y_OFS                       .equ    16 ;; add this to a screen-relative Y position to get an OAM Y position
OAM_X_OFS                       .equ    8  ;; add this to a screen-relative X position to get an OAM X position

OAM_COUNT                       .equ    40  ;; number of OAM entries in OAM RAM

;; flags
OAMF_PRI                        .equ    0x80 ;; %1000 0000 ;; Priority
OAMF_YFLIP                      .equ    0x40 ;; %0100 0000 ;; Y flip
OAMF_XFLIP                      .equ    0x20 ;; %0010 0000 ;; X flip
OAMF_PAL0                       .equ    0x00 ;; %0000 0000 ;; Palette number;; 0,1 (DMG)
OAMF_PAL1                       .equ    0x10 ;; %0001 0000 ;; Palette number;; 0,1 (DMG)
OAMF_BANK0                      .equ    0x00 ;; %0000 0000 ;; Bank number;; 0,1 (GBC)
OAMF_BANK1                      .equ    0x08 ;; %0000 1000 ;; Bank number;; 0,1 (GBC)

OAMF_PALMASK                    .equ    0x07 ;; %0000 0111 ;; Palette (GBC)

OAMB_PRI                        .equ    7 ;; Priority
OAMB_YFLIP                      .equ    6 ;; Y flip
OAMB_XFLIP                      .equ    5 ;; X flip
OAMB_PAL1                       .equ    4 ;; Palette number;; 0,1 (DMG)
OAMB_BANK1                      .equ    3 ;; Bank number;; 0,1 (GBC)


;; Deprecated constants. Please avoid using.
IEF_LCDC                        .equ    0x02 ;; %0000 0010 ;; LCDC (see STAT)
_VRAM8000                       .equ    _VRAM
_VRAM8800                       .equ    _VRAM+0x800
_VRAM9000                       .equ    _VRAM+0x1000
CART_SRAM_2KB                   .equ    1 ;; 1 incomplete bank
LCDCF_BG8800                    .equ    0x00 ;; %0000 0000 ;; BG & Window Tile Data Select
LCDCF_BG8000                    .equ    0x10 ;; %0001 0000 ;; BG & Window Tile Data Select
LCDCB_BG8000                    .equ    4 ;; BG & Window Tile Data Select
