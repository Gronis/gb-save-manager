
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
all: gb-companion_mb gb-companion

clean:
	rm -r $(OUTPUT)

.PHONY: all clean gb-companion gb-companion_mb

###############################################################################
# Build locally (inside docker container)
ifeq ($(shell which docker),)
###############################################################################

gb-companion:
	@(cd gb-companion && make OUTPUT=${OUTPUT})

gb-companion_mb:
	@(cd gba-switch2gbc && make GBC_ROM=gb-companion.gbc OUTPUT=${OUTPUT})

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

gb-companion: $(OUTPUT)/.docker
	@(docker run --rm -it -v `pwd`:/workdir gbdev bash -c "cd /workdir && make gb-companion")

gb-companion_mb: $(OUTPUT)/.docker
	@(docker run --rm -it -v `pwd`:/workdir gbdev bash -c "cd /workdir && make gb-companion_mb")

endif
endif
