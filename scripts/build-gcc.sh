#!/bin/bash
# build-gcc.sh
# Cross compiler for x86 OS development

set -e

# Configuration
TARGET=i686-elf
PREFIX="$HOME/opt/cross"
PATH="$PREFIX/bin:$PATH"

cd $HOME/src

# Download GCC
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz
tar xf gcc-13.2.0.tar.xz

# Download prerequisites
cd gcc-13.2.0
./contrib/download_prerequisites

cd ..
mkdir build-gcc
cd build-gcc

# Configure and build
../gcc-13.2.0/configure --target=$TARGET \
    --prefix="$PREFIX" \
    --disable-nls \
    --enable-languages=c,c++ \
    --without-headers \
    --enable-interwork \
    --enable-multilib \
    --disable-bootstrap \
    --disable-libssp \
    --disable-libgomp \
    --disable-libmudflap

make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc