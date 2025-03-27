
###############################################################################
# Exit if unable to build
ifeq ($(strip $(DEVKITARM)),)
ifeq ($(shell which docker),)
$(error "Error: Neither DEVKITARM or docker is found. Unable to build.")
endif
endif
###############################################################################

# Project Constants
OUTPUT			:= $(shell pwd)/build

# Build Targets
all: gb-companion_mb gb-companion_ram gb-save-manager gba-save-manager

clean:
	rm -r $(OUTPUT)


###############################################################################
# Build locally (inside docker container)
ifeq ($(shell which docker),)
###############################################################################

.PHONY: all clean gb-companion gb-companion_mb gb-companion_ram gb-save-manager gba-save-manager

gb-companion_ram:
	@(cd gb-companion && make OUTPUT=${OUTPUT} PROJECTNAME=gb-companion_ram CODE_LOC=0xC000 STACK_PTR=0xDFF0)

gb-companion:
	@(cd gb-companion && make OUTPUT=${OUTPUT})

gb-companion_mb:
	@(cd gba-switch2gbc && make GBC_ROM=gb-companion.gbc OUTPUT=${OUTPUT})

gb-save-manager:
	@(cd gb-save-manager && make OUTPUT=${OUTPUT} VRAM_ROM=gb-companion.gbc WRAM_ROM=gb-companion_ram.gbc)

gba-save-manager:
	@(cd gba-save-manager && make OUTPUT=${OUTPUT} GB_ROM=gb-companion_mb.gba)

###############################################################################
# Build using docker (on host machine)
else
ifeq ($(strip $(DEVKITARM)),)
###############################################################################

$(OUTPUT)/.:
	@mkdir -p $(OUTPUT)

$(OUTPUT)/.docker: $(OUTPUT)/. docker/* docker/*/* docker/*/*/*
	@(cd docker && docker build . -t gbdev)
	@touch $(OUTPUT)/.docker

gb-companion_ram:
	@(docker run --rm -it -v `pwd`:/workdir gbdev bash -c "cd /workdir && make gb-companion_ram")

%: %/* $(OUTPUT)/.docker %/Makefile
	@(docker run --rm -it -v `pwd`:/workdir gbdev bash -c "cd /workdir && make $@")

%_mb: $(OUTPUT)/.docker
	@(docker run --rm -it -v `pwd`:/workdir gbdev bash -c "cd /workdir && make $@")

endif
endif
