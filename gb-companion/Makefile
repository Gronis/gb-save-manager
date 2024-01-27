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
CODE_OFFSET		:=	0x0510	# 9*9*16 bytes = 1296 = 0x0510 for qr code tiles

###############################################################################
#	Project Constants
###############################################################################
PROJECTNAME		:=	gb-companion
CODE_LOC		:=	$(VRAM0)
DATA_LOC		:=	$(RAM1)
OUTPUT			:=	.

###############################################################################
#	Options/flags for tools
###############################################################################
CFLAGS			:=	--verbose -Isrc --std-c99 -msm83 --opt-code-size --peep-asm \
					--peep-return -Wa-l -Wl-m \
					-DLOCK_VERSION=13 \
					-DNO_NUMERIC \
					-DNO_ALPHANUMERIC

ASFLAGS			:=	-l -plosgffw -Isrc

LDFLAGS			:=	-mjwx -m \
					-b _CODE_LOC=$(CODE_LOC) \
					-b _ENTRYPOINT=$(CODE_LOC)+0x100 \
					-b _CODE=$(CODE_LOC)+$(CODE_OFFSET) \
					-b _DATA=$(DATA_LOC) \
					-g STACK_PTR=$(CODE_LOC)+$(CODE_OFFSET)

BNFLAGS			:=	-Z -s 0xFFFF -o $(CODE_LOC)
FXFLAGS			:=	-C -p 0 -r 0 -t GBCSAVEMANAGER -v

###############################################################################
#	 Files used to build
###############################################################################
BIN				:=	$(PROJECTNAME).gbc
SOURCES			:=	$(wildcard src/*.c)
HEADERS			:=	$(wildcard src/*.h)
ASSEMBLY		:=	$(wildcard src/*.asm)

###############################################################################
#	Derived variables
###############################################################################
BUILD_DIR		:=	$(OUTPUT)/$(PROJECTNAME)
OBJS			:=	$(SOURCES:.c=.rel) $(ASSEMBLY:.asm=.rel)
OBJS			:=	$(foreach file,$(OBJS), $(BUILD_DIR)/$(notdir $(file)))
IHX				:=	$(BIN:.gbc=.ihx)

###############################################################################
#	Build rules
###############################################################################
all: $(BUILD_DIR) $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/$(BIN:.gbc=.fixed.gbc)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/$(IHX)
	$(BN) $(BNFLAGS) $< $@

$(BUILD_DIR)/$(IHX): $(OBJS)
	$(LD) $(LDFLAGS) -i $(BUILD_DIR)/$(IHX) $(OBJS) -e

$(BUILD_DIR)/%.rel: src/%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.rel: src/%.asm Makefile
	$(AS) $(ASFLAGS) $@ $<

%.fixed.gbc: %.gbc
	@cp $< $@
	$(FX) $(FXFLAGS) $@
