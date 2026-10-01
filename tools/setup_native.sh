#!/bin/sh
# Install the EDS toolchain natively, for environments that can't run Docker (web agents, CI
# sandboxes). Mirrors docker/Dockerfile. Never needs or touches a ROM.
#
#   tools/setup_native.sh [PREFIX]        # default PREFIX: $HOME/.eds-tools
#   . "$PREFIX/env.sh"                     # then: tools/dr python3 tools/check.py <unit>
#
# Needs: a C compiler, make, git, python3 (+venv), and binutils-arm-none-eabi. On Debian/Ubuntu
# the script installs missing system packages with apt-get (via sudo when not root).
# Optional extras (decomp-permuter, objdiff-cli, m2c) install unless EDS_MINIMAL=1.
set -eu
PREFIX="${1:-$HOME/.eds-tools}"
mkdir -p "$PREFIX"

need_pkgs=""
command -v arm-none-eabi-as >/dev/null 2>&1 || need_pkgs="$need_pkgs binutils-arm-none-eabi"
command -v gcc >/dev/null 2>&1 || need_pkgs="$need_pkgs build-essential"
command -v git >/dev/null 2>&1 || need_pkgs="$need_pkgs git ca-certificates"
command -v make >/dev/null 2>&1 || need_pkgs="$need_pkgs make"
python3 -c 'import venv' >/dev/null 2>&1 || need_pkgs="$need_pkgs python3 python3-venv python3-pip"
[ -e /usr/include/png.h ] || need_pkgs="$need_pkgs libpng-dev pkg-config"
if [ -n "$need_pkgs" ]; then
    if command -v apt-get >/dev/null 2>&1; then
        SUDO=""; [ "$(id -u)" = 0 ] || SUDO="sudo"
        $SUDO apt-get update && $SUDO apt-get install -y --no-install-recommends $need_pkgs
    else
        echo "please install:$need_pkgs" >&2; exit 1
    fi
fi

# agbcc (pret's reconstruction of the GBA SDK's GCC 2.95 compilers) + its libgcc/libc.
if [ ! -x "$PREFIX/agbcc/bin/old_agbcc" ]; then
    rm -rf "$PREFIX/src-agbcc" && git clone --depth 1 https://github.com/pret/agbcc "$PREFIX/src-agbcc"
    (cd "$PREFIX/src-agbcc" && ./build.sh && mkdir -p "$PREFIX/agbcc-dest/tools" && ./install.sh "$PREFIX/agbcc-dest")
    rm -rf "$PREFIX/agbcc" && mv "$PREFIX/agbcc-dest/tools/agbcc" "$PREFIX/agbcc"
    rm -rf "$PREFIX/agbcc-dest" "$PREFIX/src-agbcc"
fi

# Python tools.
[ -x "$PREFIX/venv/bin/python3" ] || python3 -m venv "$PREFIX/venv"
"$PREFIX/venv/bin/pip" install -q capstone pyelftools

if [ "${EDS_MINIMAL:-0}" != "1" ]; then
    [ -d "$PREFIX/permuter" ] || git clone --depth 1 https://github.com/simonlindholm/decomp-permuter "$PREFIX/permuter"
    "$PREFIX/venv/bin/pip" install -q pycparser toml Levenshtein
    [ -d "$PREFIX/m2c" ] || git clone --depth 1 https://github.com/matt-kempster/m2c "$PREFIX/m2c"
    "$PREFIX/venv/bin/pip" install -q graphviz
    if [ ! -x "$PREFIX/bin/objdiff-cli" ]; then
        mkdir -p "$PREFIX/bin"
        case "$(uname -m)" in aarch64|arm64) a=linux-aarch64;; *) a=linux-x86_64;; esac
        "$PREFIX/venv/bin/python3" -c "import urllib.request,sys; urllib.request.urlretrieve(sys.argv[1], sys.argv[2])" \
            "https://github.com/encounter/objdiff/releases/download/v3.8.2/objdiff-cli-$a" "$PREFIX/bin/objdiff-cli"
        chmod +x "$PREFIX/bin/objdiff-cli"
    fi
fi

cat > "$PREFIX/env.sh" <<ENV
# Source this to use the native EDS toolchain (tools/dr then runs commands directly).
export EDS_NATIVE=1
export AGBCC_DIR="$PREFIX/agbcc"
export PERMUTER_DIR="$PREFIX/permuter"
export PATH="$PREFIX/venv/bin:$PREFIX/agbcc/bin:$PREFIX/bin:\$PATH"
ENV
echo "Toolchain ready. Run:  . \"$PREFIX/env.sh\"   then  tools/dr python3 tools/check_all.py"
