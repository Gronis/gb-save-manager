#
# General notes for the build process:
# ===================================
#
# This is just a wrapper around gb-companion that has both VRAM and RAM
# versions and chooses which one to start depending on if we are running
# on GBA or non-GBA hardware
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
PROJECTNAME		:=	gb-save-manager
STACK_PTR		:=  0xFFFE
ROM_VRAM		:=  "no ROM_VRAM defined, set using make ROM_VRAM=<project>"
ROM_RAM			:=	"no ROM_RAM defined, set using make ROM_RAM=<project>"
OUTPUT			:=	.

###############################################################################
#	 Files used to build
###############################################################################
BIN				:=	$(PROJECTNAME).gbc
VRAM_PROJ		:=	$(basename $(ROM_VRAM))

# c-files must be sorted in alphabetic order for correct memory locations 
# during link step
SOURCES			:=	$(sort $(wildcard src/*.c))
HEADERS			:=	$(wildcard src/*.h)
ASSEMBLY		:=	$(wildcard src/*.asm)

###############################################################################
#	Cartridge Header data
#	https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
###############################################################################
TITLE			:=	GBSAVEMANAG
MBC				:=  0x03	# MBC1 + RAM + BATTERY
RAM_SIZE		:=	3		# 32kB

###############################################################################
#	Derived variables
###############################################################################
BUILD_DIR		:=	$(OUTPUT)/$(PROJECTNAME)
OBJS			:=	$(SOURCES:.c=.rel) $(ASSEMBLY:.asm=.rel)
OBJS			:=	$(foreach file,$(OBJS), $(BUILD_DIR)/$(notdir $(file)))
IHX				:=	$(BIN:.gbc=.ihx)

###############################################################################
#	Options/flags for tools
###############################################################################
CFLAGS			:=	--verbose --std-c99 -msm83 --opt-code-size --peep-asm \
					-Isrc -I$(OUTPUT)/$(PROJECTNAME) -I$(OUTPUT)/$(VRAM_PROJ) \
					--peep-return -Wa-l -Wl-m \

ASFLAGS			:=	-l -plosgffw -Isrc -I$(OUTPUT)/$(PROJECTNAME)

LDFLAGS			:=	-mjwx -m \
					-b _ENTRYPOINT=0x100 \
					-b _CODE=0x150 \
					-g _STACK_PTR=$(STACK_PTR)

FXFLAGS			:=	-C -p 0 -v -m $(MBC) -O -r $(RAM_SIZE) \
					-t $(TITLE)

###############################################################################
#	Build rules
###############################################################################

all: $(BUILD_DIR) $(BUILD_DIR)/$(BIN:.gbc=.fixed.gbc) $(OUTPUT)/$(VRAM_PROJ)/$(ROM_VRAM_HEADER)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/$(IHX)
	$(BN) -Z $< $@

$(BUILD_DIR)/$(IHX): $(OBJS)
	$(LD) $(LDFLAGS) -i $@ $^ -e

$(BUILD_DIR)/%.rel: src/%.c $(HEADERS) $(OUTPUT)/$(VRAM_PROJ)/$(ROM_VRAM_HEADER) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.rel: src/%.asm Makefile
	$(AS) $(ASFLAGS) $@ $<

%_gbc.h: %.gbc
	bin2c $< $(notdir $(basename $<)) > $(basename $@).h

%_bin.h: %.bin
	bin2c $< $(notdir $(basename $<)) > $(basename $@).h

%.fixed.gbc: %.gbc
	@cp $< $@
	$(FX) $(FXFLAGS) $@

$(OUTPUT)/$(VRAM_PROJ)/$(ROM_VRAM): ../$(VRAM_PROJ)/* ../$(VRAM_PROJ)/*/*
	@(cd ../$(VRAM_PROJ) && make OUTPUT=$(OUTPUT))
