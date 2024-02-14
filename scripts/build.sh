#!/usr/bin/env bash

#
# Script: build.sh
# Description: Build alt using imgui library.
# This is an educational project for experimentation with micro transformers.
#
# Notes:
# - This build is intended for limited compute resources.
#

# Constants
readonly root_path="${PWD}"
readonly build_path="${PWD}/build"

# Check if the current directory is the root path
if [[ "${root_path##*/}" != "alt" ]]; then
    echo "Error: You are not in the root path."
    exit 1
fi

# Create the build directory if it doesn't exist
if [[ ! -d "${build_path}" ]]; then
    mkdir "${build_path}" || exit 1
fi

# Navigate to the build directory and build alt
cd "${build_path}" || exit 1
cmake .. -DALT_OPENBLAS=1
cmake --build . --config Release

# Return to the root directory
cd "${root_path}" || exit 1
