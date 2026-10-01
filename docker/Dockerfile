# EDS decomp toolchain image.
# Everything (agbcc, binutils, python analysis tools) lives in the image so the
# host stays clean. The repo is bind-mounted at /work.
FROM debian:trixie-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential binutils-arm-none-eabi git ca-certificates \
        python3 python3-venv python3-pip libpng-dev pkg-config make \
    && rm -rf /var/lib/apt/lists/*

# agbcc (GCC 2.95-based GBA SDK compiler reconstruction) + its libgcc/libc.
ARG AGBCC_REF=master
RUN git clone --depth 1 -b ${AGBCC_REF} https://github.com/pret/agbcc /tmp/agbcc \
    && cd /tmp/agbcc && ./build.sh \
    && mkdir -p /opt/agbcc-dest/tools && ./install.sh /opt/agbcc-dest \
    && mv /opt/agbcc-dest/tools/agbcc /opt/agbcc && rm -rf /opt/agbcc-dest /tmp/agbcc

# Python tools for analysis/disassembly.
RUN python3 -m venv /opt/venv && /opt/venv/bin/pip install --no-cache-dir capstone pyelftools
ENV PATH=/opt/venv/bin:/opt/agbcc/bin:$PATH
ENV AGBCC_DIR=/opt/agbcc

WORKDIR /work

# decomp-permuter: randomly rewrites a near-miss C function and scores it against the target asm.
RUN git clone --depth 1 https://github.com/simonlindholm/decomp-permuter /opt/permuter \
    && /opt/venv/bin/pip install --no-cache-dir pycparser toml Levenshtein

# objdiff-cli: per-function progress report (report.json) in the format decomp.dev consumes.
ARG OBJDIFF_VERSION=v3.8.2
RUN arch=$(uname -m); case "$arch" in aarch64|arm64) a=linux-aarch64;; *) a=linux-x86_64;; esac; \
    python3 -c "import urllib.request,sys; urllib.request.urlretrieve(sys.argv[1], '/usr/local/bin/objdiff-cli')" \
      https://github.com/encounter/objdiff/releases/download/${OBJDIFF_VERSION}/objdiff-cli-$a \
    && chmod +x /usr/local/bin/objdiff-cli

# m2c: machine-code-to-C decompiler (ARM target), for first drafts of not-yet-decompiled functions.
RUN git clone --depth 1 https://github.com/matt-kempster/m2c /opt/m2c \
    && /opt/venv/bin/pip install --no-cache-dir graphviz
