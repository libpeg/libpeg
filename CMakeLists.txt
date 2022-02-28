# Metadata: cmake version
cmake_minimum_required(VERSION 3.10)

# Metadata: project name.
project(libpeg C)

# Artifact: libpeg
set(PROJECT_LIB peg)
set(PROJECT_HEADERS
    src/types.h
    src/vm.h
)
set(PROJECT_SOURCES
    src/vm.c
)
add_library("${PROJECT_LIB}"
    SHARED
    "${PROJECT_HEADERS}"
    "${PROJECT_SOURCES}"
)

# Metadata: project version.
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Option: -DENABLE_DEBUG=On|Off
option(ENABLE_DEBUG "Enables CC -g." Off)
message("-- CC: Debug - ${ENABLE_DEBUG}.")

# Option: -DENABLE_OPTIMIZATION=On|Off
option(ENABLE_OPTIMIZATION "Enable Optimization." On)
message("-- CC: Optimization - ${ENABLE_OPTIMIZATION}.")

# CC Options:
target_compile_options("${PROJECT_LIB}" PRIVATE "-std=c99")
target_compile_options("${PROJECT_LIB}" PRIVATE "-Wall")
target_compile_options("${PROJECT_LIB}" PRIVATE "-Werror")
target_compile_options("${PROJECT_LIB}" PRIVATE "-Wextra")
target_compile_options("${PROJECT_LIB}" PRIVATE "-Wpedantic")

# CC Options:
if (ENABLE_DEBUG)
    target_compile_options("${PROJECT_LIB}" PRIVATE "-g")
endif()

# CC Options:
if (ENABLE_OPTIMIZATION)
    target_compile_options("${PROJECT_LIB}" PRIVATE "-O3")
endif()

if (ENABLE_UNITTEST)
    add_subdirectory(tests)
endif()

install(
    TARGETS "${PROJECT_LIB}"
    EXPORT "${PROJECT_LIB}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}"
)
