# EDS emulation image: mGBA (libmgba with Lua scripting, debuggers and the GDB stub) plus gdb-multiarch
# and Pillow (contact sheets).
# Used by tools/emu.py, which compiles its small libmgba harness (tools/emu/emuh.c) inside this image on
# first use. Build once with:  docker build -t eds-emu docker/emu/
# The repo is bind-mounted at /work; the ROM never enters the image.
FROM debian:trixie-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake ca-certificates curl pkg-config \
        zlib1g-dev libpng-dev liblua5.4-dev libedit-dev libelf-dev libjson-c-dev \
        python3 python3-pil fonts-dejavu-core gdb-multiarch \
    && rm -rf /var/lib/apt/lists/*

# mGBA, current stable release. Only libmgba and the headless test tools are built (no Qt/SDL/GL).
# Headers (including mgba/internal/*) and the generated mgba/flags.h are installed under /opt/mgba.
ARG MGBA_VERSION=0.10.5
RUN curl -fsSL https://github.com/mgba-emu/mgba/archive/refs/tags/${MGBA_VERSION}.tar.gz | tar xz -C /opt \
    && mv /opt/mgba-${MGBA_VERSION} /opt/mgba-src \
    && cmake -S /opt/mgba-src -B /opt/mgba-build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/mgba \
         -DBUILD_QT=OFF -DBUILD_SDL=OFF -DBUILD_GL=OFF -DBUILD_GLES2=OFF -DBUILD_GLES3=OFF -DBUILD_LTO=OFF \
         -DBUILD_SHARED=ON -DBUILD_STATIC=OFF -DBUILD_ROM_TEST=ON -DBUILD_PERF=ON \
         -DUSE_FFMPEG=OFF -DUSE_LIBZIP=OFF -DUSE_SQLITE3=OFF -DUSE_DISCORD_RPC=OFF \
         -DUSE_EPOXY=OFF -DUSE_ELF=ON -DUSE_PNG=ON -DUSE_ZLIB=ON -DUSE_EDITLINE=ON \
         -DENABLE_SCRIPTING=ON -DUSE_LUA=5.4 -DUSE_JSON_C=ON -DUSE_DEBUGGERS=ON -DUSE_GDB_STUB=ON \
    && cmake --build /opt/mgba-build -j"$(nproc)" \
    && cmake --install /opt/mgba-build \
    && echo /opt/mgba/lib > /etc/ld.so.conf.d/mgba.conf && ldconfig \
    && rm -rf /opt/mgba-build

ENV PATH=/opt/mgba/bin:$PATH
ENV MGBA_PREFIX=/opt/mgba
WORKDIR /work
