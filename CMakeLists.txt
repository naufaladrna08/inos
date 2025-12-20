cmake_minimum_required(VERSION 3.16)
project(InOS ASM C)
enable_language(C ASM)

# Generate compile_commands.json for VSCode IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ARCH x86)

# Set cross-compiler paths
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

# Cross-compiler toolchain
set(CROSS_PREFIX "$ENV{HOME}/opt/cross/bin/i686-elf-")
set(CMAKE_C_COMPILER gcc)
set(CMAKE_ASM_COMPILER ${CROSS_PREFIX}as)
set(CMAKE_LINKER ${CROSS_PREFIX}ld)
set(CMAKE_OBJCOPY ${CROSS_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${CROSS_PREFIX}objdump)

# Compiler flags
set(CMAKE_C_FLAGS "-m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -O0 -fno-pic -fno-pie -no-pie")
set(CMAKE_ASM_FLAGS "--32")

# Disable default compiler checks for cross-compilation
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)

# Add subdirectories
add_subdirectory(arch)
# add_subdirectory(kernel)  # Uncomment when ready to integrate kernel
# add_subdirectory(drivers)
# add_subdirectory(filesystems)

# Run target
# Run target
add_custom_target(run
  COMMAND qemu-system-i386 -drive format=raw,file=${CMAKE_BINARY_DIR}/arch/x86/boot.img -d cpu_reset -no-reboot
  DEPENDS boot_image
  COMMENT "Running boot.img in QEMU"
)

# Clean target
set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES 
  "${CMAKE_BINARY_DIR}/os.img"
)