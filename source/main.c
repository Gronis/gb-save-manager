
// GBA hardware docs: https://problemkaputt.de/gbatek.htm
#include <gba.h>

#define ALWAYS_INLINE                   __attribute__((always_inline)) static inline
#define PACKED                          __attribute__((packed))

#define VRAM_SIZE_GBC_MODE              0x1000
#define VRAM_CODE_SIZE                  0x7000
#define VRAM_CODE_ADDR                  ((void*)        0x06000000)
#define IWRAM_8                         ((uint32_t*)    0x03000000)

#define DC_ENABLE_CGB_MODE              0x8
#define DC_ENABLE_BG0                   0x100
#define DC_ENABLE_BG1                   0x200
#define DC_ENABLE_BG2                   0x400
#define DC_ENABLE_BG3                   0x800
#define rDISP_CNT                       ((void*)        0x4000000)

#define IMC_DISABLE_CGB_BOOT_ROM        0x8
#define IMC_ENABLE_256k_WRAM            0x20
#define IMC_WRAM_WAIT(timing)           (((uint32_t)(timing)) << 24)

#define rINTER_MEM_CNT                  (*((volatile uint32_t*)0x4000800))

#define SWI_CPUSET_FILL                 0x1000000

// #include "payload_gbc.h"
// #define PAYLOAD payload
// #define PAYLOAD_LENGTH payload_length

#include "gbc-save-manager_gbc.h"
#define PAYLOAD gbc_save_manager
#define PAYLOAD_LENGTH gbc_save_manager_length

typedef struct PACKED {
    int32_t bg_x;       // 24-bits integer + 8-bits decimal
    int32_t bg_y;       // 24-bits integer + 8-bits decimal
    int16_t scr_x;
    int16_t scr_y;
    int16_t scale_x;    // 8-bits integer + 8-bits decimal
    int16_t scale_y;    // 8-bits integer + 8-bits decimal
    uint16_t angle;     // 8-bits integer + 8-bits decimal, 0: 0 degrees, 0xFFFF: 360 degrees
    uint16_t padding;
} bg_affine_src_t;

typedef struct PACKED {
    int16_t pa;
    int16_t pb;
    int16_t pc;
    int16_t pd;
    int32_t offset_x;
    int32_t offset_y;
} bg_affine_dst_t;

void SWI_BgAffineSet(const bg_affine_src_t *src, bg_affine_dst_t *dst,
                     uint32_t count)
{
    register uint32_t src_ asm("r0") = (uint32_t)src;
    register uint32_t dst_ asm("r1") = (uint32_t)dst;
    register uint32_t count_ asm("r2") = count;
    asm volatile(
        "swi 0x0E" ::
        "r"(src_), "r"(dst_), "r"(count_) :
        "r3", "memory"
    );
}

void SWI_Halt(void)
{
    asm volatile(
        "swi 0x02" :::
        "r0", "r1", "r2", "r3", "memory"
    );
}

void SWI_CpuSet(const void *src, void *dst, uint32_t len_mode)
{
    register uint32_t src_ asm("r0") = (uint32_t)src;
    register uint32_t dst_ asm("r1") = (uint32_t)dst;
    register uint32_t len_mode_ asm("r2") = len_mode;
    asm volatile(
        "swi 0x0B" ::
        "r"(src_), "r"(dst_), "r"(len_mode_) :
        "r3", "memory"
    );
}

ALWAYS_INLINE void prepare_hardware_registers(void)
{
    bg_affine_src_t affine_src;
    bg_affine_dst_t affine_dst;
    
    {   // Set affine registers
        affine_src.bg_x = 160 / 2 << 8;
        affine_src.bg_y = 144 / 2 << 8;

        affine_src.scr_x = 40 + 160 / 2;
        affine_src.scr_y = 8 + 144 / 2;

        // This will make everything twice as big (pixel ratio 1:2)
        // This will save 9*9*8 = 648 bytes for displaying qr code
        affine_src.scale_x = 1 << 7;
        affine_src.scale_y = 1 << 7;

        // This will use default gbc scaling (pixel ratio 1:1)
        // affine_src.scale_x = 1 << 8;
        // affine_src.scale_y = 1 << 8;

        affine_src.angle = 0 << 8;
        
        // Compute the affine registers and put in affine_dst
        SWI_BgAffineSet(&affine_src, &affine_dst, 1);
    }

    {   // Reset I/O registers
        REG_BG0CNT = 0;
        REG_BG1CNT = 0;
        REG_BG3CNT = 0;

        REG_WIN0H = 0; 
        REG_WIN0V = 0; 
        REG_WIN1H = 0; 
        REG_WIN1V = 0;

        REG_WININ = 0; 
        REG_WINOUT = 0;

        REG_MOSAIC = 0; 
        REG_BLDCNT = 0;
        REG_BLDALPHA = 0;
        REG_BLDY = 0;

        REG_VCOUNT = 0;

        // Do BIOS configuration...
        REG_BG2CNT = 0x4180 | BG_MOSAIC;
    }

    {   // Reset Mosaic
        int mosaic_x = 0;
        int mosaic_y = 0;
        REG_MOSAIC = mosaic_x | mosaic_y << 4;
    }

    {   // Flush affine registers (SWI_BgAffineSet should be done at this point)
        REG_BG2PA = affine_dst.pa;
        REG_BG2PB = affine_dst.pb;
        REG_BG2PC = affine_dst.pc;
        REG_BG2PD = affine_dst.pd;
        REG_BG2X = affine_dst.offset_x;
        REG_BG2Y = affine_dst.offset_y;
    }

}

// Copy the target function into VRAM and execute it
ALWAYS_INLINE void execute_in_VRAM(const void *function)
{
    const void* _src = ((const void*)((uint32_t)function-1));
    SWI_CpuSet(_src, VRAM_CODE_ADDR, VRAM_CODE_SIZE);
    asm volatile(
        "add r1,#1\n"
        "bx r1"
    );
}

IWRAM_CODE void enter_gbc_mode(void)
{
    // Clear out IWRAM before putting any code there.
    *IWRAM_8 = 0;
    SWI_CpuSet(IWRAM_8, IWRAM_8, VRAM_SIZE_GBC_MODE | SWI_CPUSET_FILL);

    // Write gameboy color code to IWRAM. This will later turn into VRAM
    // after switch to CGB mode.
    for (int i = 0; i < PAYLOAD_LENGTH; i++){
        IWRAM_8[i] = PAYLOAD[i];
    }

    // Requres modification from inside BIOS to write CGB bit
    uint16_t disp_cnt = DC_ENABLE_BG2 | DC_ENABLE_CGB_MODE;
    SWI_CpuSet(&disp_cnt, rDISP_CNT, 1);
    
    rINTER_MEM_CNT = IMC_WRAM_WAIT(13) | IMC_ENABLE_256k_WRAM | IMC_DISABLE_CGB_BOOT_ROM;

    // This will halt the arm cpu, and if everything worked, 
    // boot into CGB mode with our code code running.
    SWI_Halt();
}

int main(void)
{
    prepare_hardware_registers();
    execute_in_VRAM(enter_gbc_mode);
    return 0;
}
