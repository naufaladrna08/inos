#!/bin/bash
# build-binutils.sh
# Cross assembler for x86 OS development

set -e

# Configuration
TARGET=i686-elf
PREFIX="$HOME/opt/cross"
PATH="$PREFIX/bin:$PATH"

# Create directories
mkdir -p $HOME/src
mkdir -p $PREFIX

cd $HOME/src

# Download binutils
wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz
tar xf binutils-2.42.tar.xz
mkdir build-binutils
cd build-binutils

# Configure and build
../binutils-2.42/configure --target=$TARGET \
    --prefix="$PREFIX" \
    --with-sysroot \
    --disable-nls \
    --disable-werror

make -j$(nproc)
make install