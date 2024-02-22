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
VRAM1			:=	0x9000
RAM0			:=	0xC000
RAM1			:=	0xD000

###############################################################################
#	Project Constants
###############################################################################
PROJECTNAME		:=	gb-companion
CODE_LOC		:=	$(VRAM1)
RAM_LOC			:=  $(RAM0)
C_CODE_OFFSET	:=	0x0150
STACK_PTR		:=  0x8FFF
OUTPUT			:=	.

###############################################################################
#	Cartridge Header data
#	https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
###############################################################################
TITLE			:=	GBSMCOMPANI
MBC				:=  0x13	# MBC3 + RAM + BATTERY
RAM_SIZE		:=	3		# 32kB

###############################################################################
#	Options/flags for tools
###############################################################################
CFLAGS			:=	--verbose --std-c99 -msm83				\
					--opt-code-size --peep-asm				\
					-Isrc -I$(OUTPUT)/$(PROJECTNAME)		\
					--peep-return -Wa-l -Wl-m				\
					-DCODE_LOC=$(CODE_LOC)					\
					-DCODE=$(CODE_LOC)+$(C_CODE_OFFSET)		\
					-DRAM_LOC=$(RAM_LOC)

ASFLAGS			:=	-l -plosgffw -Isrc -I$(OUTPUT)/$(PROJECTNAME)

LDFLAGS			:=	-mjwx -m \
					-b _ENTRYPOINT=$(CODE_LOC)+0x100		\
					-b _CODE_LOC=$(CODE_LOC)				\
					-b _RAM_LOC=$(RAM_LOC)					\
					-b _CODE=$(CODE_LOC)+$(C_CODE_OFFSET)	\
					-g CODE=$(CODE_LOC)+$(C_CODE_OFFSET)	\
					-g _STACK_PTR=$(STACK_PTR)

FXFLAGS			:=	-C -p 0 -v -m $(MBC) -O -r $(RAM_SIZE) \
					-t $(TITLE)

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
OBJS			:=  $(filter-out %/main.rel, $(OBJS))
BITMAPS			:=  $(foreach file,$(BITMAPS), $(BUILD_DIR)/$(notdir $(file)))
ASSETS			:=	$(BITMAPS:.png=_bin.h)

###############################################################################
#	Build rules
###############################################################################

all: $(BUILD_DIR) $(BUILD_DIR)/$(PROJECTNAME).fixed.gbc $(ASSETS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# main build target
$(BUILD_DIR)/$(PROJECTNAME).gbc: $(BUILD_DIR)/$(PROJECTNAME).ihx
	$(BN) -Z -o $(CODE_LOC) -s 0xFFFF $< $@
	@dd if=$@ of=$@.cut bs=4k count=1 2> /dev/null
	@mv $@.cut $@

$(BUILD_DIR)/$(PROJECTNAME).ihx: $(OBJS) $(BUILD_DIR)/main.rel
	$(LD) $(LDFLAGS) -i $@ $^ -e

# code targets for specific code locations:
$(BUILD_DIR)/prebuild.ihx: $(OBJS)
	$(LD) $(LDFLAGS) -g _main=0x0100 -i $@ $^ -e

$(BUILD_DIR)/prebuild.gbc: $(BUILD_DIR)/prebuild.ihx
	$(BN) -Z -s 0xFFFF $< $@

$(BUILD_DIR)/ram_code.gbc:	$(BUILD_DIR)/prebuild.gbc
	@dd if=$< of=$@ skip=$(shell printf "%d" $(RAM0)) bs=1 count=4k seek=0 2> /dev/null

# source code rules
$(BUILD_DIR)/%.rel: src/%.c $(HEADERS) $(ASSETS) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.rel: src/%.asm $(ASSETS) Makefile
	$(AS) $(ASFLAGS) $@ $<

# asset rules
$(BUILD_DIR)/%.png: assets/%.png
	@cp $< $@

# general rules for different file types
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

$(BUILD_DIR)/main.rel:	$(BUILD_DIR)/ram_code_gbc.h
