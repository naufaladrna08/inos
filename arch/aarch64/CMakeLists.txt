# aarch64 architecture specific configuration (TODO)
message(STATUS "Configuring aarch64 architecture")

# Set aarch64-specific flags
set(AARCH64_FLAGS "-mgeneral-regs-only")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${AARCH64_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${AARCH64_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}")

# Bootloader files
set(BOOT_SOURCE_FILES
  boot/boot.S
)

# Kernel architecture-specific files
set(ARCH_SOURCE_FILES
  # TODO: Add aarch64 specific files
)

# Linker script
set(ARCH_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker/linker.ld")

# Include directories
set(ARCH_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add include directories
include_directories(${ARCH_INCLUDE_DIRS})

# Create static library for arch-specific code
add_library(arch_aarch64 OBJECT
  ${BOOT_SOURCE_FILES}
  ${ARCH_SOURCE_FILES}
)

# Set source file properties for assembly files
set_source_files_properties(${BOOT_SOURCE_FILES} PROPERTIES LANGUAGE ASM)

# Export to parent scope
set(ARCH_SOURCE_FILES ${ARCH_SOURCE_FILES} ${BOOT_SOURCE_FILES} PARENT_SCOPE)
set(ARCH_LINKER_SCRIPT ${ARCH_LINKER_SCRIPT} PARENT_SCOPE)
set(ARCH_INCLUDE_DIRS ${ARCH_INCLUDE_DIRS} PARENT_SCOPE)