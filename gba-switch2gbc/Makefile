include $(DEVKITARM)/gba_rules

###############################################################################
# 	Tools
###############################################################################
CC				:= arm-none-eabi-gcc
LD 				:= $(CC)

###############################################################################
# 	Project Constants
###############################################################################
GBC_ROM   		:= "no GBC_ROM project defined, set using make GBC_ROM=<project>"
OUTPUT 			:= .

###############################################################################
# 	Options/flags for tools
###############################################################################
ARCH			:=	-mthumb -mthumb-interwork

CFLAGS			:=	-g -Wall -save-temps -Oz\
					-mcpu=arm7tdmi -mtune=arm7tdmi\
					-fomit-frame-pointer\
					-ffast-math \
					-fno-tree-loop-distribute-patterns \
					$(ARCH)

LDFLAGS			:=	-g $(ARCH)
BNFLAGS			:= 	-Z -s 0xFFFF -o $(CODE_LOC)
FXFLAGS			:= 	-C -p 0 -r 0 -t GBCSAVEMANAGER -v

# We don't use any functions in gba library, only definitions (registers etc)
# so no need to link to gba library
# LIBS			:=	-lgba
LIBDIRS			:=	$(LIBGBA)

###############################################################################
#   Files used to build
###############################################################################
SOURCES			:= $(wildcard src/*.c)
HEADERS			:= $(wildcard src/*.h)
ASSEMBLY 		:= $(wildcard src/*.asm)

###############################################################################
#	Derived variables
###############################################################################
GBC_PROJECT		:=	$(basename $(GBC_ROM))
BIN 			:=	$(GBC_PROJECT)_mb.gba
GBC_BUILD_DIR	:=	$(OUTPUT)/$(GBC_PROJECT)
BUILD_DIR		:=	$(OUTPUT)/$(GBC_PROJECT)_mb

OFILES			:=	$(SOURCES:.c=.o) $(ASSEMBLY:.asm=.o)
OFILES			:=	$(foreach file,$(OFILES), $(BUILD_DIR)/$(notdir $(file)))

INCLUDES		:=	$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-Isrc -I$(BUILD_DIR) -I$(GBC_BUILD_DIR)

PAYLOAD 		:=	$(subst -,_,$(GBC_PROJECT))
PAYLOAD_HEADER	:=	$(GBC_PROJECT)_gbc.h
PAYLOAD_LENGTH	:=	$(PAYLOAD)_length

DEFINES			:= 	-DPAYLOAD_HEADER=\"$(PAYLOAD_HEADER)\" \
					-DPAYLOAD=$(PAYLOAD) \
					-DPAYLOAD_LENGTH=$(PAYLOAD_LENGTH)


###############################################################################
# 	Build rules
###############################################################################

all: $(BUILD_DIR) $(BUILD_DIR)/$(BIN)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(GBC_PROJECT)_mb.elf: $(OFILES)
	$(LD) -specs=gba_mb.specs $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -Wl,-Map,$(BUILD_DIR)/$(notdir $@).map -o $@

$(OFILES): $(HEADERS) $(GBC_BUILD_DIR)/$(PAYLOAD_HEADER)

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

%_gbc.h: %.gbc
	bin2c $< $(notdir $(basename $<)) > $(basename $@).h

$(GBC_BUILD_DIR)/$(GBC_ROM): ../$(GBC_PROJECT)/* ../$(GBC_PROJECT)/*/*
	@(cd ../$(GBC_PROJECT) && make OUTPUT=$(OUTPUT))
