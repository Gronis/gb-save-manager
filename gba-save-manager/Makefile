include $(DEVKITARM)/gba_rules

###############################################################################
# 	Tools
###############################################################################
CC					:= arm-none-eabi-gcc
LD 					:= $(CC)

###############################################################################
# 	Project Constants
###############################################################################
PROJECTNAME			:= gba-save-manager
GB_MB_ROM   		:= gb-companion_mb.gba
OUTPUT 				:= .

###############################################################################
# 	Options/flags for tools
###############################################################################
ARCH				:=	-mthumb -mthumb-interwork

CFLAGS				:=	-g -Wall -save-temps -Oz\
						-mcpu=arm7tdmi -mtune=arm7tdmi\
						-fomit-frame-pointer\
						-ffast-math \
						-fno-tree-loop-distribute-patterns \
						$(ARCH)

LDFLAGS				:=	-g $(ARCH)
# BNFLAGS			:= 	-Z -s 0xFFFF -o $(CODE_LOC)
# FXFLAGS			:= 	-C -p 0 -r 0 -t GBASAVEMANAGER -v

# We don't use any functions in gba library, only definitions (registers etc)
# so no need to link to gba library
# LIBS				:=	-lgba
LIBDIRS				:=	$(LIBGBA)

###############################################################################
#   Files used to build
###############################################################################
SOURCES				:= $(wildcard src/*.c)
HEADERS				:= $(wildcard src/*.h)
ASSEMBLY 			:= $(wildcard src/*.asm)

###############################################################################
#	Derived variables
###############################################################################
BIN 				:=	$(PROJECTNAME).gba
BUILD_DIR			:=	$(OUTPUT)/$(PROJECTNAME)

OFILES				:=	$(SOURCES:.c=.o) $(ASSEMBLY:.asm=.o)
OFILES				:=	$(foreach file,$(OFILES), $(BUILD_DIR)/$(notdir $(file)))

GB_MB_PROJ			:=	$(basename $(GB_MB_ROM))
GB_MB_BUILD_DIR		:=	$(OUTPUT)/$(GB_MB_PROJ)
PAYLOAD_GB_HEADER	:=	$(GB_MB_PROJ)_gba.h

INCLUDES			:=	$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
						-Isrc -I$(BUILD_DIR) -I$(GB_MB_BUILD_DIR)


DEFINES				:= 	


###############################################################################
# 	Build rules
###############################################################################

all: $(BUILD_DIR) $(BUILD_DIR)/$(BIN)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(PROJECTNAME).elf: $(OFILES)
	$(LD) -specs=gba_mb.specs $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -Wl,-Map,$(BUILD_DIR)/$(notdir $@).map -o $@

$(OFILES): $(HEADERS) $(GB_MB_BUILD_DIR)/$(PAYLOAD_GB_HEADER)

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

%_gba.h: %.gba
	bin2c $< $(notdir $(basename $<)) > $(basename $@).h

$(GB_MB_BUILD_DIR)/$(GB_MB_ROM): ../$(GB_MB_PROJ:_mb=)/* ../$(GB_MB_PROJ:_mb=)/*/*
	@(cd .. && make $(GB_MB_PROJ) OUTPUT=$(OUTPUT))
