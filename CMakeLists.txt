cmake_minimum_required(VERSION 3.12)
project("alt.cpp")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT XCODE AND NOT MSVC AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(ALT_STANDALONE ON)
else()
    set(ALT_STANDALONE OFF)
endif()

# general
# TODO: tests
# option(ALT_BUILD_TESTS       "alt: build tests"          ${ALT_STANDALONE})
# TODO: examples
# option(ALT_BUILD_EXAMPLES    "alt: build examples"       ${ALT_STANDALONE})
# TODO: shared libs
option(ALT_SHARED_LIBS       "alt: build shared libs"    OFF)
option(ALT_OPENBLAS          "alt: openblas backend"     OFF)
option(ALT_VULKAN            "alt: vulkan backend"       OFF)

if(ALT_OPENBLAS)
	message("Use CUBLAS as backend stable-diffusion")
    set(ALT_OPENBLAS ON)
    add_definitions(-DALT_USE_OPENBLAS)
endif()

if(ALT_VULKAN)
	message("Use CUBLAS as backend stable-diffusion")
    set(ALT_VULKAN ON)
    add_definitions(-DALT_USE_VULKAN)
endif()

# Add the ggml submodule directory
add_subdirectory(imgui)

# Define the main executable
add_executable(alt src/main.c)

# Link the ggml library
target_link_libraries(alt PUBLIC imgui)

# Specify include directories for this specific target
target_include_directories(alt PUBLIC 
    ${PROJECT_SOURCE_DIR}/imgui 
    ${PROJECT_SOURCE_DIR}/imgui/backends
    ${PROJECT_SOURCE_DIR}/include
)

# Set the output directory for built binaries
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Set the output directory for the imgui shared object
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
