#ifndef HARDWARE_H
#define HARDWARE_H

// This file is converted from "hardware.inc" assembly definition to work with c
//
// Gameboy Hardware definitions
//
// Based on Jones` hardware.inc
// And based on Carsten Sorensen`s ideas.
//
// Rev 1.1 - 15-Jul-97 : Added define check
// Rev 1.2 - 18-Jul-97 : Added revision check macro
// Rev 1.3 - 19-Jul-97 : Modified for RGBASM V1.05
// Rev 1.4 - 27-Jul-97 : Modified for new subroutine prefixes
// Rev 1.5 - 15-Aug-97 : Added _HRAM, PAD, CART defines
//                     :  and Nintendo Logo
// Rev 1.6 - 30-Nov-97 : Added rDIV, rTIMA, rTMA, & rTAC
// Rev 1.7 - 31-Jan-98 : Added _SCRN0, _SCRN1
// Rev 1.8 - 15-Feb-98 : Added rSB, rSC
// Rev 1.9 - 16-Feb-98 : Converted I/O registers to $FFXX format
// Rev 2.0 -           : Added GBC registers
// Rev 2.1 -           : Added MBC5 & cart RAM enable/disable defines
// Rev 2.2 -           : Fixed NR42,NR43, & NR44 equates
// Rev 2.3 -           : Fixed incorrect _HRAM equate
// Rev 2.4 - 27-Apr-13 : Added some cart defines (AntonioND)
// Rev 2.5 - 03-May-15 : Fixed format (AntonioND)
// Rev 2.6 - 09-Apr-16 : Added GBC OAM and cart defines (AntonioND)
// Rev 2.7 - 19-Jan-19 : Added rPCMXX (ISSOtm)
// Rev 2.8 - 03-Feb-19 : Added audio registers flags (Álvaro Cuesta)
// Rev 2.9 - 28-Feb-20 : Added utility rP1 constants
// Rev 3.0 - 27-Aug-20 : Register ordering, byte-based sizes, OAM additions, general cleanup (Blitter Object)
// Rev 4.0 - 03-May-21 : Updated to use RGBDS 0.5.0 syntax, changed IEF_LCDC to IEF_STAT (Eievui)
// Rev 4.1 - 16-Aug-21 : Added more flags, bit number defines, and offset constants for OAM and window positions (rondnelson99)
// Rev 4.2 - 04-Sep-21 : Added CH3- and CH4-specific audio registers flags (ISSOtm)
// Rev 4.3 - 07-Nov-21 : Deprecate VRAM address constants (Eievui)
// Rev 4.4 - 11-Jan-22 : Deprecate VRAM CART_SRAM_2KB constant (avivace)
// Rev 4.5 - 03-Mar-22 : Added bit number definitions for OCPS, BCPS and LCDC (sukus)
// Rev 4.6 - 15-Jun-22 : Added MBC3 registers and special values
// Rev 4.7.0 - 27-Jun-22 : Added alternate names for some constants
// Rev 4.7.1 - 05-Jul-22 : Added RPB_LED_ON constant
// Rev 4.8.0 - 25-Oct-22 : Changed background addressing constants (zlago)


#ifndef HARDWARE_INC
#define HARDWARE_INC 1
#endif

#include "types.h"

//#**************************************************************************
//#
//# General memory region constants
//#
//#**************************************************************************

#define _VRAM         ((uint8_t*)0x8000) // 0x8000->0x9FFF
#define _SCRN0        ((uint8_t*)0x9800) // 0x9800->0x9BFF
#define _SCRN1        ((uint8_t*)0x9C00) // 0x9C00->0x9FFF
#define _SRAM         ((uint8_t*)0xA000) // 0xA000->0xBFFF
#define _RAM          ((uint8_t*)0xC000) // 0xC000->0xCFFF / 0xC000->0xDFFF
#define _RAMBANK      ((uint8_t*)0xD000) // 0xD000->0xDFFF
#define _OAMRAM       ((uint8_t*)0xFE00) // 0xFE00->0xFE9F
#define _IO           ((uint8_t*)0xFF00) // 0xFF00->0xFF7F,0xFFFF
#define _AUD3WAVERAM  ((uint8_t*)0xFF30) // 0xFF30->0xFF3F
#define _HRAM         ((uint8_t*)0xFF80) // 0xFF80->0xFFFE


//#**************************************************************************
//#
//# MBC registers
//#
//#**************************************************************************

// *** Common ***

// --
// -- RAMG (0x0000-0x1FFF)
// -- Controls whether access to SRAM (and the MBC3 RTC registers) is allowed (W)
// --
#define rRAMG              0x0000

#define CART_SRAM_ENABLE   0x0A
#define CART_SRAM_DISABLE  0x00


// --
// -- ROMB0 (0x2000-0x3FFF)
// -- Selects which ROM bank is mapped to the ROMX space (0x4000-0x7FFF) (W)
// --
// -- The range of accepted values, as well as the behavior of writing 0x00,
// -- varies depending on the MBC.
// --
#define rROMB0  ((uint8_t*)0x2000)

// --
// -- RAMB (0x4000-0x5FFF)
// -- Selects which SRAM bank is mapped to the SRAM space (0xA000-0xBFFF) (W)
// --
// -- The range of accepted values varies depending on the cartridge configuration.
// --
#define rRAMB  ((uint8_t*)0x4000)


// *** MBC3-specific registers ***

// Write one of these to rRAMG to map the corresponding RTC register to all SRAM space
#define RTC_S   0x08    // Seconds  (0-59)
#define RTC_M   0x09    // Minutes  (0-59)
#define RTC_H   0x0A    // Hours    (0-23)
#define RTC_DL  0x0B    // Lower 8 bits of Day Counter (0x00-0xFF)
#define RTC_DH  0x0C    // Bit 7 - Day Counter Carry Bit (1=Counter Overflow)
                        // Bit 6 - Halt (0=Active, 1=Stop Timer)
                        // Bit 0 - Most significant bit of Day Counter (Bit 8)


// --
// -- RTCLATCH (0x6000-0x7FFF)
// -- Write 0x00 then 0x01 to latch the current time into the RTC registers (W)
// --
#define rRTCLATCH  0x6000


// *** MBC5-specific register ***

// --
// -- ROMB1 (0x3000-0x3FFF)
// -- A 9th bit that "extends" ROMB0 if more than 256 banks are present (W)
// --
// -- Also note that rROMB0 thus only spans 0x2000-0x2FFF.
// --
#define rROMB1  0x3000


// Bit 3 of RAMB enables the rumble motor (if any)
#define CART_RUMBLE_ON  1 << 3


//#**************************************************************************
//#
//# Memory-mapped registers
//#
//#**************************************************************************

// --
// -- P1 (0xFF00)
// -- Register for reading joy pad info. (R/W)
// --
#define rP1  ((uint8_t*)0xFF00)

#define P1F_5  0x20 // %0010 0000 // P15 out port, set to 0 to get buttons
#define P1F_4  0x10 // %0001 0000 // P14 out port, set to 0 to get dpad
#define P1F_3  0x08 // %0000 1000 // P13 in port
#define P1F_2  0x04 // %0000 0100 // P12 in port
#define P1F_1  0x02 // %0000 0010 // P11 in port
#define P1F_0  0x01 // %0000 0001 // P10 in port

#define P1F_GET_DPAD  P1F_5
#define P1F_GET_BTN   P1F_4
#define P1F_GET_NONE  P1F_4 | P1F_5


// --
// -- SB (0xFF01)
// -- Serial Transfer Data (R/W)
// --
#define rSB  ((uint8_t*)0xFF01)


// --
// -- SC (0xFF02)
// -- Serial I/O Control (R/W)
// --
#define rSC  ((uint8_t*)0xFF02)

#define SCF_START   0x80 // %10000000 // Transfer Start Flag (1=Transfer in progress, or requested)
#define SCF_SPEED   0x02 // %00000010 // Clock Speed (0=Normal, 1=Fast) ** CGB Mode Only **
#define SCF_SOURCE  0x01 // %00000001 // Shift Clock (0=External Clock, 1=Internal Clock)

#define SCB_START   7
#define SCB_SPEED   1
#define SCB_SOURCE  0

// --
// -- DIV (0xFF04)
// -- Divider register (R/W)
// --
#define rDIV  ((uint8_t*)0xFF04)


// --
// -- TIMA (0xFF05)
// -- Timer counter (R/W)
// --
#define rTIMA  ((uint8_t*)0xFF05)


// --
// -- TMA (0xFF06)
// -- Timer modulo (R/W)
// --
#define rTMA  ((uint8_t*)0xFF06)


// --
// -- TAC (0xFF07)
// -- Timer control (R/W)
// --
#define rTAC  ((uint8_t*)0xFF07)

#define TACF_START   0x04 // %00000100
#define TACF_STOP    0x00 // %00000000
#define TACF_4KHZ    0x00 // %00000000
#define TACF_16KHZ   0x03 // %00000011
#define TACF_65KHZ   0x02 // %00000010
#define TACF_262KHZ  0x01 // %00000001

#define TACB_START   2


// --
// -- IF (0xFF0F)
// -- Interrupt Flag (R/W)
// --
#define rIF  ((uint8_t*)0xFF0F)


// --
// -- AUD1SWEEP/NR10 (0xFF10)
// -- Sweep register (R/W)
// --
// -- Bit 6-4 - Sweep Time
// -- Bit 3   - Sweep Increase/Decrease
// --           0: Addition    (frequency increases?)
// --           1: Subtraction (frequency increases?)
// -- Bit 2-0 - Number of sweep shift (# 0-7)
// -- Sweep Time: (n*7.8ms)
// --
#define rNR10  ((uint8_t*)0xFF10)
#define rAUD1SWEEP  ((uint8_t*)rNR10)

#define AUD1SWEEP_UP    0x00 // %00000000
#define AUD1SWEEP_DOWN  0x08 // %00001000


// --
// -- AUD1LEN/NR11 (0xFF11)
// -- Sound length/Wave pattern duty (R/W)
// --
// -- Bit 7-6 - Wave Pattern Duty (00:12.5% 01:25% 10:50% 11:75%)
// -- Bit 5-0 - Sound length data (# 0-63)
// --
#define rNR11  ((uint8_t*)0xFF11)
#define rAUD1LEN  ((uint8_t*)rNR11)


// --
// -- AUD1ENV/NR12 (0xFF12)
// -- Envelope (R/W)
// --
// -- Bit 7-4 - Initial value of envelope
// -- Bit 3   - Envelope UP/DOWN
// --           0: Decrease
// --           1: Range of increase
// -- Bit 2-0 - Number of envelope sweep (# 0-7)
// --
#define rNR12  ((uint8_t*)0xFF12)
#define rAUD1ENV  ((uint8_t*)rNR12)


// --
// -- AUD1LOW/NR13 (0xFF13)
// -- Frequency low byte (W)
// --
#define rNR13  ((uint8_t*)0xFF13)
#define rAUD1LOW  ((uint8_t*)rNR13)


// --
// -- AUD1HIGH/NR14 (0xFF14)
// -- Frequency high byte (W)
// --
// -- Bit 7   - Initial (when set, sound restarts)
// -- Bit 6   - Counter/consecutive selection
// -- Bit 2-0 - Frequency`s higher 3 bits
// --
#define rNR14  ((uint8_t*)0xFF14)
#define rAUD1HIGH  ((uint8_t*)rNR14)


// --
// -- AUD2LEN/NR21 (0xFF16)
// -- Sound Length// Wave Pattern Duty (R/W)
// --
// -- see AUD1LEN for info
// --
#define rNR21  ((uint8_t*)0xFF16)
#define rAUD2LEN  ((uint8_t*)rNR21)


// --
// -- AUD2ENV/NR22 (0xFF17)
// -- Envelope (R/W)
// --
// -- see AUD1ENV for info
// --
#define rNR22  ((uint8_t*)0xFF17)
#define rAUD2ENV  ((uint8_t*)rNR22)


// --
// -- AUD2LOW/NR23 (0xFF18)
// -- Frequency low byte (W)
// --
#define rNR23  ((uint8_t*)0xFF18)
#define rAUD2LOW  ((uint8_t*)rNR23)


// --
// -- AUD2HIGH/NR24 (0xFF19)
// -- Frequency high byte (W)
// --
// -- see AUD1HIGH for info
// --
#define rNR24  ((uint8_t*)0xFF19)
#define rAUD2HIGH  ((uint8_t*)rNR24)


// --
// -- AUD3ENA/NR30 (0xFF1A)
// -- Sound on/off (R/W)
// --
// -- Bit 7   - Sound ON/OFF (1=ON,0=OFF)
// --
#define rNR30  ((uint8_t*)0xFF1A)
#define rAUD3ENA  ((uint8_t*)rNR30)

#define AUD3ENA_OFF  0x00 // %00000000
#define AUD3ENA_ON   0x80 // %10000000


// --
// -- AUD3LEN/NR31 (0xFF1B)
// -- Sound length (R/W)
// --
// -- Bit 7-0 - Sound length
// --
#define rNR31  ((uint8_t*)0xFF1B)
#define rAUD3LEN  ((uint8_t*)rNR31)


// --
// -- AUD3LEVEL/NR32 (0xFF1C)
// -- Select output level
// --
// -- Bit 6-5 - Select output level
// --           00: 0/1 (mute)
// --           01: 1/1
// --           10: 1/2
// --           11: 1/4
// --
#define rNR32  ((uint8_t*)0xFF1C)
#define rAUD3LEVEL  ((uint8_t*)rNR32)

#define AUD3LEVEL_MUTE  0x00 // %0000 0000
#define AUD3LEVEL_100   0x20 // %0010 0000
#define AUD3LEVEL_50    0x40 // %0100 0000
#define AUD3LEVEL_25    0x60 // %0110 0000


// --
// -- AUD3LOW/NR33 (0xFF1D)
// -- Frequency low byte (W)
// --
// -- see AUD1LOW for info
// --
#define rNR33  ((uint8_t*)0xFF1D)
#define rAUD3LOW  ((uint8_t*)rNR33)


// --
// -- AUD3HIGH/NR34 (0xFF1E)
// -- Frequency high byte (W)
// --
// -- see AUD1HIGH for info
// --
#define rNR34  ((uint8_t*)0xFF1E)
#define rAUD3HIGH  ((uint8_t*)rNR34)


// --
// -- AUD4LEN/NR41 (0xFF20)
// -- Sound length (R/W)
// --
// -- Bit 5-0 - Sound length data (# 0-63)
// --
#define rNR41  ((uint8_t*)0xFF20)
#define rAUD4LEN  ((uint8_t*)rNR41)


// --
// -- AUD4ENV/NR42 (0xFF21)
// -- Envelope (R/W)
// --
// -- see AUD1ENV for info
// --
#define rNR42  ((uint8_t*)0xFF21)
#define rAUD4ENV  ((uint8_t*)rNR42)


// --
// -- AUD4POLY/NR43 (0xFF22)
// -- Polynomial counter (R/W)
// --
// -- Bit 7-4 - Selection of the shift clock frequency of the (scf)
// --           polynomial counter (0000-1101)
// --           freq=drf*1/2^scf (not sure)
// -- Bit 3 -   Selection of the polynomial counter`s step
// --           0: 15 steps
// --           1: 7 steps
// -- Bit 2-0 - Selection of the dividing ratio of frequencies (drf)
// --           000: f/4   001: f/8   010: f/16  011: f/24
// --           100: f/32  101: f/40  110: f/48  111: f/56  (f=4.194304 Mhz)
// --
#define rNR43  ((uint8_t*)0xFF22)
#define rAUD4POLY  ((uint8_t*)rNR43)

#define AUD4POLY_15STEP  0x00 // %00000000
#define AUD4POLY_7STEP   0x08 // %00001000


// --
// -- AUD4GO/NR44 (0xFF23)
// --
// -- Bit 7 -   Initial (when set, sound restarts)
// -- Bit 6 -   Counter/consecutive selection
// --
#define rNR44  ((uint8_t*)0xFF23)
#define rAUD4GO  ((uint8_t*)rNR44)





// --
// -- AUDVOL/NR50 (0xFF24)
// -- Channel control / ON-OFF / Volume (R/W)
// --
// -- Bit 7   - Vin->SO2 ON/OFF (left)
// -- Bit 6-4 - SO2 output level (left speaker) (# 0-7)
// -- Bit 3   - Vin->SO1 ON/OFF (right)
// -- Bit 2-0 - SO1 output level (right speaker) (# 0-7)
// --
#define rNR50  ((uint8_t*)0xFF24)
#define rAUDVOL  ((uint8_t*)rNR50)

#define AUDVOL_VIN_LEFT   0x80 // %10000000 // SO2
#define AUDVOL_VIN_RIGHT  0x08 // %00001000 // SO1


// --
// -- AUDTERM/NR51 (0xFF25)
// -- Selection of Sound output terminal (R/W)
// --
// -- Bit 7   - Output channel 4 to SO2 terminal (left)
// -- Bit 6   - Output channel 3 to SO2 terminal (left)
// -- Bit 5   - Output channel 2 to SO2 terminal (left)
// -- Bit 4   - Output channel 1 to SO2 terminal (left)
// -- Bit 3   - Output channel 4 to SO1 terminal (right)
// -- Bit 2   - Output channel 3 to SO1 terminal (right)
// -- Bit 1   - Output channel 2 to SO1 terminal (right)
// -- Bit 0   - Output channel 1 to SO1 terminal (right)
// --
#define rNR51  ((uint8_t*)0xFF25)
#define rAUDTERM  ((uint8_t*)rNR51)

// SO2
#define AUDTERM_4_LEFT   0x80 // %1000 0000
#define AUDTERM_3_LEFT   0x40 // %0100 0000
#define AUDTERM_2_LEFT   0x20 // %0010 0000
#define AUDTERM_1_LEFT   0x10 // %0001 0000
// SO1
#define AUDTERM_4_RIGHT  0x08 // %0000 1000
#define AUDTERM_3_RIGHT  0x04 // %0000 0100
#define AUDTERM_2_RIGHT  0x02 // %0000 0010
#define AUDTERM_1_RIGHT  0x01 // %0000 0001


// --
// -- AUDENA/NR52 (0xFF26)
// -- Sound on/off (R/W)
// --
// -- Bit 7   - All sound on/off (sets all audio regs to 0!)
// -- Bit 3   - Sound 4 ON flag (read only)
// -- Bit 2   - Sound 3 ON flag (read only)
// -- Bit 1   - Sound 2 ON flag (read only)
// -- Bit 0   - Sound 1 ON flag (read only)
// --
#define rNR52  ((uint8_t*)0xFF26)
#define rAUDENA  ((uint8_t*)rNR52)

#define AUDENA_ON     0x80 // %10000000
#define AUDENA_OFF    0x00 // %00000000  // sets all audio regs to 0!


// --
// -- LCDC (0xFF40)
// -- LCD Control (R/W)
// --
#define rLCDC  ((uint8_t*)0xFF40)

#define LCDCF_OFF      0x00 // %0000 0000 // LCD Control Operation
#define LCDCF_ON       0x80 // %1000 0000 // LCD Control Operation
#define LCDCF_WIN9800  0x00 // %0000 0000 // Window Tile Map Display Select
#define LCDCF_WIN9C00  0x40 // %0100 0000 // Window Tile Map Display Select
#define LCDCF_WINOFF   0x00 // %0000 0000 // Window Display
#define LCDCF_WINON    0x20 // %0010 0000 // Window Display
#define LCDCF_BLK21    0x00 // %0000 0000 // BG & Window Tile Data Select
#define LCDCF_BLK01    0x10 // %0001 0000 // BG & Window Tile Data Select
#define LCDCF_BG9800   0x00 // %0000 0000 // BG Tile Map Display Select
#define LCDCF_BG9C00   0x08 // %0000 1000 // BG Tile Map Display Select
#define LCDCF_OBJ8     0x00 // %0000 0000 // OBJ Construction
#define LCDCF_OBJ16    0x04 // %0000 0100 // OBJ Construction
#define LCDCF_OBJOFF   0x00 // %0000 0000 // OBJ Display
#define LCDCF_OBJON    0x02 // %0000 0010 // OBJ Display
#define LCDCF_BGOFF    0x00 // %0000 0000 // BG Display
#define LCDCF_BGON     0x01 // %0000 0001 // BG Display

#define LCDCB_ON       7 // LCD Control Operation
#define LCDCB_WIN9C00  6 // Window Tile Map Display Select
#define LCDCB_WINON    5 // Window Display
#define LCDCB_BLKS     4 // BG & Window Tile Data Select
#define LCDCB_BG9C00   3 // BG Tile Map Display Select
#define LCDCB_OBJ16    2 // OBJ Construction
#define LCDCB_OBJON    1 // OBJ Display
#define LCDCB_BGON     0 // BG Display
// "Window Character Data Select" follows BG


// --
// -- STAT (0xFF41)
// -- LCDC Status   (R/W)
// --
#define rSTAT  ((uint8_t*)0xFF41)

#define STATF_LYC       0x40 // %0100 0000 // LYC=LY Coincidence (Selectable)
#define STATF_MODE10    0x20 // %0010 0000 // Mode 10
#define STATF_MODE01    0x10 // %0001 0000 // Mode 01 (V-Blank)
#define STATF_MODE00    0x08 // %0000 1000 // Mode 00 (H-Blank)
#define STATF_LYCF      0x04 // %0000 0100 // Coincidence Flag
#define STATF_HBL       0x00 // %0000 0000 // H-Blank
#define STATF_VBL       0x01 // %0000 0001 // V-Blank
#define STATF_OAM       0x02 // %0000 0010 // OAM-RAM is used by system
#define STATF_LCD       0x03 // %0000 0011 // Both OAM and VRAM used by system
#define STATF_BUSY      0x02 // %0000 0010 // When set, VRAM access is unsafe

#define STATB_LYC       6
#define STATB_MODE10    5
#define STATB_MODE01    4
#define STATB_MODE00    3
#define STATB_LYCF      2
#define STATB_BUSY      1

// --
// -- SCY (0xFF42)
// -- Scroll Y (R/W)
// --
#define rSCY  ((uint8_t*)0xFF42)


// --
// -- SCX (0xFF43)
// -- Scroll X (R/W)
// --
#define rSCX  ((uint8_t*)0xFF43)


// --
// -- LY (0xFF44)
// -- LCDC Y-Coordinate (R)
// --
// -- Values range from 0->153. 144->153 is the VBlank period.
// --
#define rLY  ((uint8_t*)0xFF44)


// --
// -- LYC (0xFF45)
// -- LY Compare (R/W)
// --
// -- When LY==LYC, STATF_LYCF will be set in STAT
// --
#define rLYC  ((uint8_t*)0xFF45)


// --
// -- DMA (0xFF46)
// -- DMA Transfer and Start Address (W)
// --
#define rDMA  ((uint8_t*)0xFF46)


// --
// -- BGP (0xFF47)
// -- BG Palette Data (W)
// --
// -- Bit 7-6 - Intensity for %11
// -- Bit 5-4 - Intensity for %10
// -- Bit 3-2 - Intensity for %01
// -- Bit 1-0 - Intensity for %00
// --
#define rBGP  ((uint8_t*)0xFF47)


// --
// -- OBP0 (0xFF48)
// -- Object Palette 0 Data (W)
// --
// -- See BGP for info
// --
#define rOBP0  ((uint8_t*)0xFF48)


// --
// -- OBP1 (0xFF49)
// -- Object Palette 1 Data (W)
// --
// -- See BGP for info
// --
#define rOBP1  ((uint8_t*)0xFF49)


// --
// -- WY (0xFF4A)
// -- Window Y Position (R/W)
// --
// -- 0 <= WY <= 143
// -- When WY = 0, the window is displayed from the top edge of the LCD screen.
// --
#define rWY  ((uint8_t*)0xFF4A)


// --
// -- WX (0xFF4B)
// -- Window X Position (R/W)
// --
// -- 7 <= WX <= 166
// -- When WX = 7, the window is displayed from the left edge of the LCD screen.
// -- Values of 0-6 and 166 are unreliable due to hardware bugs.
// --
#define rWX  ((uint8_t*)0xFF4B)

#define WX_OFS  7 // add this to a screen position to get a WX position

// -- This unknown register is at the tail end of the LCD register space. 
// -- It may possibly lockdown advanced color hardware features from being
// -- accessed by a game in DMG mode. This could be tested. The CGB boot ROM
// -- sets this to $80 if a CGB game is inserted, or $04 if a DMG game is
// -- inserted.
#define rLCDMODE ((uint8_t*)0xFF4C)

// --
// -- SPEED (0xFF4D)
// -- Select CPU Speed (R/W)
// --
#define rKEY1  ((uint8_t*)0xFF4D)
#define rSPD   ((uint8_t*)rKEY1)

#define KEY1F_DBLSPEED  0x80 // %10000000 // 0=Normal Speed, 1=Double Speed (R)
#define KEY1F_PREPARE   0x01 // %00000001 // 0=No, 1=Prepare (R/W)


// --
// -- VBK (0xFF4F)
// -- Select Video RAM Bank (R/W)
// --
// -- Bit 0 - Bank Specification (0: Specify Bank 0// 1: Specify Bank 1)
// --
#define rVBK  ((uint8_t*)0xFF4F)

// -- Assigning this address to value 0x11 disables boot ROM. This is done by
// -- GBC bios during boot process
#define rBLCK ((uint8_t*)0xFF50)

// --
// -- HDMA1 (0xFF51)
// -- High byte for Horizontal Blanking/General Purpose DMA source address (W)
// -- CGB Mode Only
// --
#define rHDMA1  ((uint8_t*)0xFF51)


// --
// -- HDMA2 (0xFF52)
// -- Low byte for Horizontal Blanking/General Purpose DMA source address (W)
// -- CGB Mode Only
// --
#define rHDMA2  ((uint8_t*)0xFF52)


// --
// -- HDMA3 (0xFF53)
// -- High byte for Horizontal Blanking/General Purpose DMA destination address (W)
// -- CGB Mode Only
// --
#define rHDMA3  ((uint8_t*)0xFF53)


// --
// -- HDMA4 (0xFF54)
// -- Low byte for Horizontal Blanking/General Purpose DMA destination address (W)
// -- CGB Mode Only
// --
#define rHDMA4  ((uint8_t*)0xFF54)


// --
// -- HDMA5 (0xFF55)
// -- Transfer length (in tiles minus 1)/mode/start for Horizontal Blanking, General Purpose DMA (R/W)
// -- CGB Mode Only
// --
#define rHDMA5  ((uint8_t*)0xFF55)

#define HDMA5F_MODE_GP   0x00 // %00000000 // General Purpose DMA (W)
#define HDMA5F_MODE_HBL  0x80 // %10000000 // HBlank DMA (W)
#define HDMA5B_MODE      7                 // DMA mode select (W)

// -- Once DMA has started, use HDMA5F_BUSY to check when the transfer is complete
#define HDMA5F_BUSY  %10000000 // 0=Busy (DMA still in progress), 1=Transfer complete (R)


// --
// -- RP (0xFF56)
// -- Infrared Communications Port (R/W)
// -- CGB Mode Only
// --
#define rRP  ((uint8_t*)0xFF56)

#define RPF_ENREAD    0xC0 // %11000000
#define RPF_DATAIN    0x02 // %00000010 // 0=Receiving IR Signal, 1=Normal
#define RPF_WRITE_HI  0x01 // %00000001
#define RPF_WRITE_LO  0x00 // %00000000

#define RPB_LED_ON    0
#define RPB_DATAIN    1


// --
// -- BCPS/BGPI (0xFF68)
// -- Background Color Palette Specification (aka Background Palette Index) (R/W)
// --
#define rBCPS  ((uint8_t*)0xFF68)
#define rBGPI  ((uint8_t*)rBCPS)

#define BCPSF_AUTOINC  0x80 // %10000000 // Auto Increment (0=Disabled, 1=Increment after Writing)
#define BCPSB_AUTOINC  7
#define BGPIF_AUTOINC  BCPSF_AUTOINC
#define BGPIB_AUTOINC  BCPSB_AUTOINC


// --
// -- BCPD/BGPD (0xFF69)
// -- Background Color Palette Data (aka Background Palette Data) (R/W)
// --
#define rBCPD  ((uint8_t*)0xFF69)
#define rBGPD  ((uint8_t*)rBCPD)


// --
// -- OCPS/OBPI (0xFF6A)
// -- Object Color Palette Specification (aka Object Background Palette Index) (R/W)
// --
#define rOCPS  ((uint8_t*)0xFF6A)
#define rOBPI  ((uint8_t*)rOCPS)

#define OCPSF_AUTOINC  0x80 // %10000000 // Auto Increment (0=Disabled, 1=Increment after Writing)
#define OCPSB_AUTOINC  7
#define OBPIF_AUTOINC  OCPSF_AUTOINC
#define OBPIB_AUTOINC  OCPSB_AUTOINC


// --
// -- OCPD/OBPD (0xFF6B)
// -- Object Color Palette Data (aka Object Background Palette Data) (R/W)
// --
#define rOCPD  ((uint8_t*)0xFF6B)
#define rOBPD  ((uint8_t*)rOCPD)


// --
// -- SMBK/SVBK (0xFF70)
// -- Select Main RAM Bank (R/W)
// --
// -- Bit 2-0 - Bank Specification (0,1: Specify Bank 1// 2-7: Specify Banks 2-7)
// --
#define rSVBK  ((uint8_t*)0xFF70)
#define rSMBK  ((uint8_t*)rSVBK)


// --
// -- PCM12 (0xFF76)
// -- Sound channel 1&2 PCM amplitude (R)
// --
// -- Bit 7-4 - Copy of sound channel 2`s PCM amplitude
// -- Bit 3-0 - Copy of sound channel 1`s PCM amplitude
// --
#define rPCM12  ((uint8_t*)0xFF76)


// --
// -- PCM34 (0xFF77)
// -- Sound channel 3&4 PCM amplitude (R)
// --
// -- Bit 7-4 - Copy of sound channel 4`s PCM amplitude
// -- Bit 3-0 - Copy of sound channel 3`s PCM amplitude
// --
#define rPCM34  ((uint8_t*)0xFF77)


// --
// -- IE (0xFFFF)
// -- Interrupt Enable (R/W)
// --
#define rIE  ((uint8_t*)0xFFFF)

#define IEF_HILO    0x10 // %0001 0000 // Transition from High to Low of Pin number P10-P13
#define IEF_SERIAL  0x08 // %0000 1000 // Serial I/O transfer end
#define IEF_TIMER   0x04 // %0000 0100 // Timer Overflow
#define IEF_STAT    0x02 // %0000 0010 // STAT
#define IEF_VBLANK  0x01 // %0000 0001 // V-Blank

#define IEB_HILO    4
#define IEB_SERIAL  3
#define IEB_TIMER   2
#define IEB_STAT    1
#define IEB_VBLANK  0


//#**************************************************************************
//#
//# Flags common to multiple sound channels
//#
//#**************************************************************************

// --
// -- Square wave duty cycle
// --
// -- Can be used with AUD1LEN and AUD2LEN
// -- See AUD1LEN for more info
// --
#define AUDLEN_DUTY_12_5     0x00 // %0000 0000 // 12.5%
#define AUDLEN_DUTY_25       0x40 // %0100 0000 // 25%
#define AUDLEN_DUTY_50       0x80 // %1000 0000 // 50%
#define AUDLEN_DUTY_75       0xC0 // %1100 0000 // 75%


// --
// -- Audio envelope flags
// --
// -- Can be used with AUD1ENV, AUD2ENV, AUD4ENV
// -- See AUD1ENV for more info
// --
#define AUDENV_UP            0x08 // %0000 1000
#define AUDENV_DOWN          0x00 // %0000 0000


// --
// -- Audio trigger flags
// --
// -- Can be used with AUD1HIGH, AUD2HIGH, AUD3HIGH
// -- See AUD1HIGH for more info
// --
#define AUDHIGH_RESTART      0x80 // %1000 0000
#define AUDHIGH_LENGTH_ON    0x40 // %0100 0000
#define AUDHIGH_LENGTH_OFF   0x00 // %0000 0000


//#**************************************************************************
//#
//# CPU values on bootup (a=type, b=qualifier)
//#
//#**************************************************************************

#define BOOTUP_A_DMG     0x01 // Dot Matrix Game
#define BOOTUP_A_CGB     0x11 // Color GameBoy
#define BOOTUP_A_MGB     0xFF // Mini GameBoy (Pocket GameBoy)

// if a=BOOTUP_A_CGB, bit 0 in b can be checked to determine if real CGB or
// other system running in GBC mode
#define BOOTUP_B_CGB     0x00 // %0000 0000
#define BOOTUP_B_AGB     0x01 // %0000 0001   // GBA, GBA SP, Game Boy Player, or New GBA SP


//#**************************************************************************
//#
//# Header
//#
//#**************************************************************************

//#
//# Nintendo scrolling logo
//# (Code won`t work on a real GameBoy)
//# (if next lines are altered.)
#define NINTENDO_LOGO \
    0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D \
    0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99 \
    0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E

// 0x0143 Color GameBoy compatibility code
#define CART_COMPATIBLE_DMG      0x00
#define CART_COMPATIBLE_DMG_GBC  0x80
#define CART_COMPATIBLE_GBC      0xC0

// 0x0146 GameBoy/Super GameBoy indicator
#define CART_INDICATOR_GB        0x00
#define CART_INDICATOR_SGB       0x03

// 0x0147 Cartridge type
#define CART_ROM                      0x00
#define CART_ROM_MBC1                 0x01
#define CART_ROM_MBC1_RAM             0x02
#define CART_ROM_MBC1_RAM_BAT         0x03
#define CART_ROM_MBC2                 0x05
#define CART_ROM_MBC2_BAT             0x06
#define CART_ROM_RAM                  0x08
#define CART_ROM_RAM_BAT              0x09
#define CART_ROM_MMM01                0x0B
#define CART_ROM_MMM01_RAM            0x0C
#define CART_ROM_MMM01_RAM_BAT        0x0D
#define CART_ROM_MBC3_BAT_RTC         0x0F
#define CART_ROM_MBC3_RAM_BAT_RTC     0x10
#define CART_ROM_MBC3                 0x11
#define CART_ROM_MBC3_RAM             0x12
#define CART_ROM_MBC3_RAM_BAT         0x13
#define CART_ROM_MBC5                 0x19
#define CART_ROM_MBC5_BAT             0x1A
#define CART_ROM_MBC5_RAM_BAT         0x1B
#define CART_ROM_MBC5_RUMBLE          0x1C
#define CART_ROM_MBC5_RAM_RUMBLE      0x1D
#define CART_ROM_MBC5_RAM_BAT_RUMBLE  0x1E
#define CART_ROM_MBC7_RAM_BAT_GYRO    0x22
#define CART_ROM_POCKET_CAMERA        0xFC
#define CART_ROM_BANDAI_TAMA5         0xFD
#define CART_ROM_HUDSON_HUC3          0xFE
#define CART_ROM_HUDSON_HUC1          0xFF

// 0x0148 ROM size
// these are kilobytes
#define CART_ROM_32KB    0x00 // 2 banks
#define CART_ROM_64KB    0x01 // 4 banks
#define CART_ROM_128KB   0x02 // 8 banks
#define CART_ROM_256KB   0x03 // 16 banks
#define CART_ROM_512KB   0x04 // 32 banks
#define CART_ROM_1024KB  0x05 // 64 banks
#define CART_ROM_2048KB  0x06 // 128 banks
#define CART_ROM_4096KB  0x07 // 256 banks
#define CART_ROM_8192KB  0x08 // 512 banks
#define CART_ROM_1152KB  0x52 // 72 banks
#define CART_ROM_1280KB  0x53 // 80 banks
#define CART_ROM_1536KB  0x54 // 96 banks

// 0x0149 SRAM size
// these are kilobytes
#define CART_SRAM_NONE   0
#define CART_SRAM_8KB    2 // 1 bank
#define CART_SRAM_32KB   3 // 4 banks
#define CART_SRAM_128KB  4 // 16 banks

// 0x014A Destination code
#define CART_DEST_JAPANESE      0x00
#define CART_DEST_NON_JAPANESE  0x01


//#**************************************************************************
//#
//# Keypad related
//#
//#**************************************************************************

#define PADF_DOWN    0x80
#define PADF_UP      0x40
#define PADF_LEFT    0x20
#define PADF_RIGHT   0x10
#define PADF_START   0x08
#define PADF_SELECT  0x04
#define PADF_B       0x02
#define PADF_A       0x01

#define PADB_DOWN    0x7
#define PADB_UP      0x6
#define PADB_LEFT    0x5
#define PADB_RIGHT   0x4
#define PADB_START   0x3
#define PADB_SELECT  0x2
#define PADB_B       0x1
#define PADB_A       0x0


//#**************************************************************************
//#
//# Screen related
//#
//#**************************************************************************

#define SCRN_X     160 // Width of screen in pixels
#define SCRN_Y     144 // Height of screen in pixels. Also corresponds to the value in LY at the beginning of VBlank.
#define SCRN_X_B   20  // Width of screen in bytes
#define SCRN_Y_B   18  // Height of screen in bytes

#define SCRN_VX    256 // Virtual width of screen in pixels
#define SCRN_VY    256 // Virtual height of screen in pixels
#define SCRN_VX_B  32  // Virtual width of screen in bytes
#define SCRN_VY_B  32  // Virtual height of screen in bytes


//#**************************************************************************
//#
//# OAM related
//#
//#**************************************************************************

// OAM attributes
// each entry in OAM RAM is 4 bytes (sizeof_OAM_ATTRS)
// !! Not sure what this is and why its here -> RSRESET
#define OAMA_Y              RB  1   // y pos plus 16
#define OAMA_X              RB  1   // x pos plus 8
#define OAMA_TILEID         RB  1   // tile id
#define OAMA_FLAGS          RB  1   // flags (see below)
#define sizeof_OAM_ATTRS    RB  0

#define OAM_Y_OFS  16 // add this to a screen-relative Y position to get an OAM Y position
#define OAM_X_OFS  8  // add this to a screen-relative X position to get an OAM X position

#define OAM_COUNT            40  // number of OAM entries in OAM RAM

// flags
#define OAMF_PRI         0x80 // %1000 0000 // Priority
#define OAMF_YFLIP       0x40 // %0100 0000 // Y flip
#define OAMF_XFLIP       0x20 // %0010 0000 // X flip
#define OAMF_PAL0        0x00 // %0000 0000 // Palette number// 0,1 (DMG)
#define OAMF_PAL1        0x10 // %0001 0000 // Palette number// 0,1 (DMG)
#define OAMF_BANK0       0x00 // %0000 0000 // Bank number// 0,1 (GBC)
#define OAMF_BANK1       0x08 // %0000 1000 // Bank number// 0,1 (GBC)

#define OAMF_PALMASK     0x07 // %0000 0111 // Palette (GBC)

#define OAMB_PRI         7 // Priority
#define OAMB_YFLIP       6 // Y flip
#define OAMB_XFLIP       5 // X flip
#define OAMB_PAL1        4 // Palette number// 0,1 (DMG)
#define OAMB_BANK1       3 // Bank number// 0,1 (GBC)


// Deprecated constants. Please avoid using.

#define IEF_LCDC       0x02 // %0000 0010 // LCDC (see STAT)
#define _VRAM8000     _VRAM
#define _VRAM8800     _VRAM+0x800
#define _VRAM9000     _VRAM+0x1000
#define CART_SRAM_2KB     1 // 1 incomplete bank
#define LCDCF_BG8800   0x00 // %0000 0000 // BG & Window Tile Data Select
#define LCDCF_BG8000   0x10 // %0001 0000 // BG & Window Tile Data Select
#define LCDCB_BG8000      4 // BG & Window Tile Data Select


#endif // HARDWARE_H
