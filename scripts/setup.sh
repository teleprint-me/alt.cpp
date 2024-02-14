#!/usr/bin/env bash

#
# Script: setup.sh
# Description: Apply GGML Python dependencies.
# This is an educational project for experimentation with a micro transformer.
#

# Constants
readonly root_path="${PWD}"
readonly submodule="${PWD}/ggml"
readonly venv_name=".venv"

# Check if the current directory is the root path
if [[ "${root_path##*/}" != "alt.cpp" ]]; then
    echo "Error: You are not in the root path of alt.cpp."
    exit 1
fi

# Check if virtualenv is installed
if ! command -v virtualenv &> /dev/null; then
    echo "Error: virtualenv is not installed. Please install it before running this script."
    exit 1
fi

# Create a virtual environment if it doesn't exist
if [[ ! -d "${venv_name}" ]]; then
    virtualenv "${venv_name}" || exit 1
fi

# Activate the virtual environment
source "${venv_name}/bin/activate" || exit 1

# Check if requirements.txt exists
if [[ ! -f "${submodule}/requirements.txt" ]]; then
    echo "Error: requirements.txt file not found in the submodule."
    exit 1
fi

# Install dependencies from requirements.txt
pip install -r "${submodule}/requirements.txt" || exit 1

echo "GGML Python dependencies have been successfully installed."
