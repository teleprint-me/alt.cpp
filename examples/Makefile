CC = gcc
CXX = g++

im:
	$(CXX) ../imgui/imgui.cpp \
	../imgui/imgui_draw.cpp \
	../imgui/imgui_tables.cpp \
	../imgui/imgui_widgets.cpp \
	../imgui/backends/imgui_impl_glfw.cpp \
	../imgui/backends/imgui_impl_opengl3.cpp \
	../examples/imgui-example.cpp -o ../examples/im.bin \
	-I../ -L../ -I../imgui -I../imgui/backends \
	-lglfw -lGLEW -lGL -std=c++17 
