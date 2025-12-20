# x86 architecture specific configuration
message(STATUS "Configuring x86 architecture")

# Set paths
set(ARCH_X86_SOURCE_DIR ${CMAKE_SOURCE_DIR}/arch/x86)
set(BOOT_DIR ${ARCH_X86_SOURCE_DIR}/boot)
set(LINKER_DIR ${ARCH_X86_SOURCE_DIR}/linker)
set(INCLUDE_DIR ${ARCH_X86_SOURCE_DIR}/include)

message("INCLUDE" ${INCLUDE_DIR})

# Include directories (This isn't work, don't care for now)
include_directories(${INCLUDE_DIR})

# The boot produce three binary files: boot, setup and main and the end
# of the build process we merge it to one image file. 

# ===== BOOT SECTOR (boot.bin) =====
# Assemble boot.s to boot.o
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot.o
  COMMAND ${CMAKE_ASM_COMPILER} ${CMAKE_ASM_FLAGS} -c ${BOOT_DIR}/boot.s -o ${CMAKE_CURRENT_BINARY_DIR}/boot.o
  DEPENDS ${BOOT_DIR}/boot.s
  COMMENT "Assembling boot.s"
)

# Link boot.o to boot.bin (512 bytes, sector 1)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot.bin
  COMMAND ${CMAKE_LINKER} -m elf_i386 -Ttext 0x7C00 --oformat binary ${CMAKE_CURRENT_BINARY_DIR}/boot.o -o ${CMAKE_CURRENT_BINARY_DIR}/boot.bin
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/boot.o
  COMMENT "Linking boot.bin"
)

add_custom_target(arch_x86_boot DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/boot.bin)

# Compile and link setup.elf
add_executable(setup_elf boot/setup.s boot/main.c boot/tty.c boot/idt.c)
target_link_options(setup_elf PRIVATE -T ${LINKER_DIR}/link.ld)

add_custom_command(
  OUTPUT setup.elf
  COMMAND ${CMAKE_LINKER}
          -m elf_i386
          -nostdlib
          -static
          -T ${LINKER_DIR}/link.ld
          setup.o main.o tty.o
          -o setup.elf
  DEPENDS setup.o main.o tty.o
)

# Convert setup.elf to setup.bin
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.bin
  COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_CURRENT_BINARY_DIR}/setup_elf ${CMAKE_CURRENT_BINARY_DIR}/setup.bin
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/setup_elf
  COMMENT "Creating setup.bin"
)

add_custom_target(arch_x86_setup DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/setup.bin)

# ===== BOOT IMAGE (boot.img) =====
# Create boot.img containing boot.bin and setup.bin
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot.img
  COMMAND dd if=/dev/zero of=${CMAKE_CURRENT_BINARY_DIR}/boot.img bs=512 count=2880
  COMMAND dd if=${CMAKE_CURRENT_BINARY_DIR}/boot.bin of=${CMAKE_CURRENT_BINARY_DIR}/boot.img bs=512 count=1 conv=notrunc
  COMMAND dd if=${CMAKE_CURRENT_BINARY_DIR}/setup.bin of=${CMAKE_CURRENT_BINARY_DIR}/boot.img bs=512 seek=1 conv=notrunc
  DEPENDS arch_x86_boot arch_x86_setup
  COMMENT "Creating boot.img disk image"
)

add_custom_target(boot_image ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/boot.img)

# ===== UTILITY TARGETS =====
# Run target for testing
add_custom_target(run-boot
  COMMAND qemu-system-i386 -drive format=raw,file=${CMAKE_CURRENT_BINARY_DIR}/boot.img -d cpu_reset -no-reboot
  DEPENDS boot_image
  COMMENT "Running boot.img in QEMU"
)
