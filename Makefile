# Compiler settings
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -Wall -O2
CXXFLAGS = -Wall -Wextra -std=c++17 -I./imgui -I./imgui/backends

# Linker flags
LDFLAGS = -lglfw -lGLEW -lGL -lvulkan

# Target executable name
EXECUTABLE = alt  # alt is the name of the application

# Explicit source files

IMGUI = imgui/imgui.cpp \
	imgui/backends/imgui_impl_glfw.cpp\
	imgui/backends/imgui_impl_opengl3.cpp \
	imgui/imgui_draw.cpp \
	imgui/imgui_widgets.cpp \
	imgui/imgui_tables.cpp

ALT = source/linear.cpp source/alt.cpp

SOURCES = $(IMGUI) $(ALT)

# Explicit object files
OBJECTS = \
	imgui/imgui.o \
	imgui/backends/imgui_impl_glfw.o \
	imgui/backends/imgui_impl_opengl3.o \
	imgui/imgui_draw.o \
	imgui/imgui_widgets.o \
	imgui/imgui_tables.o \
	source/linear.o \
	source/alt.o

# Build the executable
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Build ImGUI
imgui/imgui.o: imgui/imgui.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

imgui/backends/imgui_impl_glfw.o: imgui/backends/imgui_impl_glfw.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

imgui/backends/imgui_impl_opengl3.o: imgui/backends/imgui_impl_opengl3.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

imgui/imgui_draw.o: imgui/imgui_draw.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

imgui/imgui_widgets.o: imgui/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

imgui/imgui_tables.o: imgui/imgui_tables.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build Alt
source/linear.o: source/linear.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# `alt` is the "main" binary
source/alt.o: source/alt.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Phony targets for cleanliness and maintenance
.PHONY: clean submodules

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

submodules:
	git submodule update --init --recursive
