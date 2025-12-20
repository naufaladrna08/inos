# Architecture-specific code
message(STATUS "Configuring architecture: ${ARCH}")

# Add the selected architecture subdirectory
if(ARCH STREQUAL "x86")
  add_subdirectory(${ARCH})
elseif(ARCH STREQUAL "aarch64")
  add_subdirectory(${ARCH})
else()
  message(FATAL_ERROR "Unsupported architecture: ${ARCH}")
endif()

# Export architecture variables to parent scope
set(ARCH_SOURCE_FILES ${ARCH_SOURCE_FILES} PARENT_SCOPE)
set(ARCH_LINKER_SCRIPT ${ARCH_LINKER_SCRIPT} PARENT_SCOPE)
set(ARCH_INCLUDE_DIRS ${ARCH_INCLUDE_DIRS} PARENT_SCOPE)