###################################################################################
### Build rbgds which is used to build assembly code for the Gameboy
###################################################################################
FROM devkitpro/devkitarm AS build-rgbds

RUN apt-get update && \
    apt-get install sudo make cmake gcc build-essential -y

RUN git clone https://github.com/gbdev/rgbds.git rgbds && \
    cd rgbds && \
    git checkout v0.7.0

WORKDIR rgbds

RUN apt-get update && \
    apt-get install libpng-dev bison -y

RUN make -j$(nproc) CXXFLAGS="-O3 -flto -DNDEBUG -static" PKG_CONFIG="pkg-config --static" Q=

RUN mkdir -p /opt/rgbds/bin && \  
    cp rgbasm rgblink rgbfix rgbgfx /opt/rgbds/bin

###################################################################################
### Install bin2c
###################################################################################
FROM devkitpro/devkitarm AS build-bin2c

RUN apt-get update && \
    apt-get install sudo make cmake gcc build-essential -y

COPY ./bin2c bin2c
WORKDIR bin2c
RUN make -j$(nproc)
RUN mkdir -p /opt/bin2c/bin && \
    cp bin2c /opt/bin2c/bin

###################################################################################
### Install sdcc
###################################################################################
FROM devkitpro/devkitarm AS build-sdcc

RUN apt-get update \
  && apt-get install -y bison flex g++ make patch wget xz-utils zlib1g-dev libboost-all-dev \
  && rm -rf /var/lib/apt/lists/*

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
FROM devkitpro/devkitarm

RUN apt-get update \
  && apt-get install -y make \
  && rm -rf /var/lib/apt/lists/*

COPY --from=build-sdcc  /opt  /opt
COPY --from=build-rgbds /opt  /opt
COPY --from=build-bin2c /opt  /opt

ENV PATH /opt/sdcc/bin:$PATH
ENV PATH /opt/rgbds/bin:$PATH
ENV PATH /opt/bin2c/bin:$PATH

WORKDIR /workdir

CMD bash