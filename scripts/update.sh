#!/usr/bin/env bash

#
# Script: update.sh
# Description: Update the imgui submodule to the latest version.
# This script ensures that the submodule is up to date.
#

# Constants
readonly submodule="${PWD}/imgui"

# Check if the current directory is the root path
if [[ "${PWD##*/}" != "alt" ]]; then
    echo "Error: You are not in the root path of phi.cpp."
    exit 1
fi

# Check if the submodule directory exists
if [[ ! -d "${submodule}" ]]; then
    echo "Error: The imgui submodule directory does not exist."
    exit 1
fi

# Intialize and/or Update the submodule
# NOTE: https://git-scm.com/book/en/v2/Git-Tools-Submodules
git submodule update --init --recursive --remote "${submodule}"

echo "IMGUI submodule has been successfully updated."
