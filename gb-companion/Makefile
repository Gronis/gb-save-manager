#
# General notes for the build process:
# ===================================
#
# Code is expected to run either in VRAM or RAM on GB/GBC or GBA in GBC mode
# For GB/GBC code lives in ROM at boot, it is then moved to RAM or VRAM
# depending on the build. 
#
# For GBA, only VRAM version works, because RAM is inacessable while there
# is no game cartridge inserted. For GB, VRAM version is unusable because
# it turns the LCD on and off very fast in order to execute code on VRAM
# while still show an image.
#
# At some point, when we know we have a cartridge inside, execution will
# change to RAM only. Therefore, we need to build in two steps. Once for
# ram_code, where the code is linked to execute in RAM. The second time we
# include the ram_code but put it with the rest of the code. It is then 
# moved during runtime to RAM. This is for VRAM build only. RAM build do
# not need this, but the Makefile is the same so all the same build steps
# still apply.
#


###############################################################################
#	Tools
###############################################################################
CC				:=	sdcc
AS				:=	sdasgb
LD				:=	sdldgb
BN				:=	makebin
FX				:=	rgbfix

###############################################################################
#	Static Addresses
###############################################################################
VRAM0			:=	0x8000
VRAM1			:=	0x8800
RAM0			:=	0xC000
RAM1			:=	0xD000

###############################################################################
#	Project Constants
###############################################################################
PROJECTNAME		:=	gb-companion
CODE_LOC		:=	$(VRAM0)
RAM_LOC			:=  $(RAM0)
CODE_OFFSET		:=	0x01F8  # Leave space for gfx, expand if more gxf is needed
STACK_PTR		:=  0x9800
DATA_OFFSET		:=  0x1000
OUTPUT			:=	.

###############################################################################
#	Options/flags for tools
###############################################################################
CFLAGS			:=	--verbose --std-c99 -msm83 --opt-code-size --peep-asm \
					-Isrc -I$(OUTPUT)/$(PROJECTNAME) \
					--peep-return -Wa-l -Wl-m \
					-DRAM_LOC=$(RAM_LOC) \
					-DCODE_LOC=$(CODE_LOC) \
					-DLOCK_VERSION=13 \
					-DNO_NUMERIC \
					-DNO_ALPHANUMERIC

ASFLAGS			:=	-l -plosgffw -Isrc -I$(OUTPUT)/$(PROJECTNAME)

LDFLAGS			:=	-mjwx -m \
					-b _CODE_LOC=$(CODE_LOC) \
					-b _RAM_LOC=$(RAM_LOC) \
					-b _INIT_LOC=$(CODE_LOC)+0x150 \
					-b _CODE=$(CODE_LOC)+$(CODE_OFFSET) \
					-b _DATA=$(CODE_LOC)+$(DATA_OFFSET) \
					-b _ENTRYPOINT=$(CODE_LOC)+0x100 \
					-g _STACK_PTR=$(STACK_PTR)

FXFLAGS			:=	-C -p 0 -r 0 -t GBSMCOMPANION -v

###############################################################################
#	 Files used to build
###############################################################################
BIN				:=	$(PROJECTNAME).gbc

# c-files must be sorted in alphabetic order for correct memory locations 
# during link step
SOURCES			:=	$(sort $(wildcard src/*.c))
HEADERS			:=	$(wildcard src/*.h)
ASSEMBLY		:=	$(wildcard src/*.asm)
BITMAPS			:=	$(wildcard assets/*.png)

###############################################################################
#	Derived variables
###############################################################################
BUILD_DIR		:=	$(OUTPUT)/$(PROJECTNAME)
OBJS			:=	$(SOURCES:.c=.rel) $(ASSEMBLY:.asm=.rel)
OBJS			:=	$(foreach file,$(OBJS), $(BUILD_DIR)/$(notdir $(file)))
IHX				:=	$(BIN:.gbc=.ihx)
BITMAPS			:=  $(foreach file,$(BITMAPS), $(BUILD_DIR)/$(notdir $(file)))
ASSETS			:=	$(BITMAPS:.png=_bin.h)

###############################################################################
#	Build rules
###############################################################################

all: $(BUILD_DIR) $(BUILD_DIR)/$(BIN:.gbc=.fixed.gbc) $(ASSETS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/$(IHX)
	$(BN) -Z -s 0xFFFF -o $(CODE_LOC) $< $@
	@dd if=$@ of=$@.cut bs=16k count=1 2> /dev/null
	@mv $@.cut $@

$(BUILD_DIR)/ram_code.gbc: $(BUILD_DIR)/ram_code.ihx
	$(BN) -Z -s $(RAM1) -o $(RAM0) $< $@

$(BUILD_DIR)/$(IHX): $(OBJS)
	$(LD) $(LDFLAGS) -i $@ $^ -e

$(BUILD_DIR)/ram_code.ihx: $(OBJS)
	$(LD) $(LDFLAGS) -g _main=0x0100 -i $@ $^ -e

$(BUILD_DIR)/%.rel: src/%.c $(HEADERS) $(ASSETS) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.rel: src/%.asm $(ASSETS) Makefile
	$(AS) $(ASFLAGS) $@ $<

$(BUILD_DIR)/%.png: assets/%.png
	@cp $< $@

%_gbc.h: %.gbc
	bin2c $< $(notdir $(basename $<)) > $(basename $@).h

%_bin.h: %.bin
	bin2c $< $(notdir $(basename $<)) > $(basename $@).h

%.bin: %.png
	mkfont $<

%.fixed.gbc: %.gbc
	@cp $< $@
	$(FX) $(FXFLAGS) $@

###############################################################################
#	Specific dependencies for certain files
###############################################################################

$(BUILD_DIR)/main.rel: \
	$(BUILD_DIR)/ram_code_gbc.h
