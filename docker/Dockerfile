FROM devkitpro/devkitarm:20240202 as base

###################################################################################
### Build rbgds which is used to build assembly code for the Gameboy
###################################################################################
FROM base AS build-rgbds

RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        sudo make cmake gcc build-essential \
        libpng-dev bison

RUN git clone https://github.com/gbdev/rgbds.git rgbds && \
    cd rgbds && \
    git checkout v0.7.0

WORKDIR rgbds

RUN make -j$(nproc) CXXFLAGS="-O3 -flto -DNDEBUG -static" PKG_CONFIG="pkg-config --static" Q=

RUN mkdir -p /opt/rgbds/bin && \  
    cp rgbasm rgblink rgbfix rgbgfx /opt/rgbds/bin

###################################################################################
### Install bin2c
###################################################################################
FROM base AS build-bin2c

RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        sudo make cmake gcc build-essential

COPY ./bin2c bin2c
WORKDIR bin2c
RUN make -j$(nproc)
RUN mkdir -p /opt/bin2c/bin && \
    cp bin2c /opt/bin2c/bin

###################################################################################
### Rust base
###################################################################################
FROM base AS rust-base

RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        build-essential \
        curl \
        pkg-config

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

###################################################################################
### Install mkfont
###################################################################################
FROM rust-base as build-mkfont

COPY ./mkfont mkfont
WORKDIR mkfont
RUN . "$HOME/.cargo/env" && cargo build --release
RUN mkdir -p /opt/mkfont/bin && \
    cp target/release/mkfont /opt/mkfont/bin

###################################################################################
### Install sdcc
###################################################################################
FROM base AS build-sdcc

RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        bison flex g++ make patch wget xz-utils zlib1g-dev libboost-all-dev && \
    rm -rf /var/lib/apt/lists/*

RUN wget https://sourceforge.net/projects/sdcc/files/sdcc/4.4.0/sdcc-src-4.4.0-rc2.tar.bz2
RUN tar xf sdcc-src-4.4.0-rc2.tar.bz2
RUN mkdir -p /opt/sdcc
RUN cd sdcc-4.4.0-rc2 && \
    ./configure \
        --prefix=/opt/sdcc \
        --disable-pic14-port \
        --disable-pic16-port && \
    make -j$(nproc)
RUN cd sdcc-4.4.0-rc2 && \
    make install

###################################################################################
### Final build image
###################################################################################
FROM base

RUN apt-get update \
  && apt-get install -y make \
  && rm -rf /var/lib/apt/lists/*

COPY --from=build-sdcc      /opt  /opt
COPY --from=build-rgbds     /opt  /opt
COPY --from=build-bin2c     /opt  /opt
COPY --from=build-mkfont    /opt  /opt

ENV PATH /opt/sdcc/bin:$PATH
ENV PATH /opt/rgbds/bin:$PATH
ENV PATH /opt/bin2c/bin:$PATH
ENV PATH /opt/mkfont/bin:$PATH
ENV PATH /opt/devkitpro/devkitARM/bin:$PATH

WORKDIR /workdir

CMD bash