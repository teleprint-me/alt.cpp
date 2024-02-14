# Compiler settings
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -Wall -O2
CXXFLAGS = $(CFLAGS)

# Linker flags
LDFLAGS = 

# OpenCL and Vulkan Libraries
VULKAN_LIBS = -lvulkan

# Targets
all: list_vulkan_devices

list_vulkan_devices: list_vulkan_devices.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(VULKAN_LIBS)

.PHONY: clean

clean:
	rm -f list_vulkan_devices
