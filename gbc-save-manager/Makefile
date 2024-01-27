###############################################################################
# 	Tools
###############################################################################
CC			:= sdcc
AS          := sdasgb
LD			:= sdldgb
BN			:= makebin
FX			:= rgbfix
OUTPUT 		:= .

###############################################################################
# 	Static Addresses
###############################################################################
VRAM0       := 0x8000
VRAM1       := 0x8800
RAM0        := 0xC000
RAM1        := 0xD000
CODE_OFFSET := 0x0510  # 9*9*16 bytes = 1296 = 0x0510 for qr code tiles

###############################################################################
# 	Project Constants
###############################################################################
PROJECTNAME	:= gbc-save-manager
CODE_LOC    := $(VRAM0)
DATA_LOC    := $(RAM1)

###############################################################################
# 	Options/flags for tools
###############################################################################
CFLAGS		:= 	--verbose -Isrc --std-c99 -msm83 --opt-code-size --peep-asm \
				--peep-return -Wa-l -Wl-m \
				-DLOCK_VERSION=13 \
				-DNO_NUMERIC \
				-DNO_ALPHANUMERIC

ASFLAGS		:= 	-l -plosgffw -Isrc

LDFLAGS		:= 	-mjwx -m \
				-b _CODE_LOC=$(CODE_LOC) \
				-b _ENTRYPOINT=$(CODE_LOC)+0x100 \
				-b _CODE=$(CODE_LOC)+$(CODE_OFFSET) \
				-b _DATA=$(DATA_LOC) \
				-g STACK_PTR=$(CODE_LOC)+$(CODE_OFFSET)

BNFLAGS		:= 	-Z -s 0xFFFF -o $(CODE_LOC)
FXFLAGS		:= 	-C -p 0 -r 0 -t GBCSAVEMANAGER -v

###############################################################################
#   Files used to build
###############################################################################
BIN 		:= $(PROJECTNAME).gbc
SOURCES		:= $(wildcard src/*.c)
HEADERS		:= $(wildcard src/*.h)
ASSEMBLY 	:= $(wildcard src/*.asm)
LINKER		:= src/linker.lk

###############################################################################
#	Derived variables
###############################################################################
OBJS		:= $(SOURCES:.c=.rel) $(ASSEMBLY:.asm=.rel)
OBJS		:= $(foreach file,$(OBJS), $(OUTPUT)/$(notdir $(file)))
IHX			:= $(BIN:.gbc=.ihx)

###############################################################################
# 	Build rules
###############################################################################
all: $(OUTPUT)/$(BIN) $(OUTPUT)/$(BIN:.gbc=_gbc.h) $(OUTPUT)/$(BIN:.gbc=.fixed.gbc)

$(OUTPUT)/$(BIN): $(OUTPUT)/$(IHX)
	$(BN) $(BNFLAGS) $< $@

$(OUTPUT)/$(IHX): $(OBJS) $(LINKER)
	$(LD) $(LDFLAGS) -i $(OUTPUT)/$(IHX) -f $(LINKER) $(OBJS) -e

$(OUTPUT)/%.rel: src/%.c $(HEADERS) Makefile
	echo $(OBJS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT)/%.rel: src/%.asm Makefile
	echo $(OBJS)
	$(AS) $(ASFLAGS) $@ $<

%_gbc.h: %.gbc
	bin2c $< $(notdir $(basename $<)) > $(basename $@).h

%.fixed.gbc: %.gbc
	cp $< $@
	$(FX) $(FXFLAGS) $@

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm
