/*
 * Copyright © 2024 Austin Berrio
 *
 * alt.cpp/source/alt.cpp
 */
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"
#include "../include/logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

GLFWwindow* create_parent_glfw_window(
    int width, int height, const char* title, GLFWmonitor* monitor, struct Logger* logger
) {
    if (!glfwInit()) {
        LOG(logger, LOG_LEVEL_ERROR, "Failed to initialize GLFW\n");
        return NULL;
    }

    GLFWwindow* parent_window = glfwCreateWindow(width, height, title, monitor, NULL);

    if (parent_window == NULL) {
        LOG(logger, LOG_LEVEL_ERROR, "Failed to create a GLFW window\n");
        return NULL;
    }

    glfwMakeContextCurrent(parent_window);
    glfwSwapInterval(0); // Enable vsync

    if (glewInit() != GLEW_OK) {
        LOG(logger, LOG_LEVEL_ERROR, "Failed to initialize GLEW\n");
        return NULL;
    }

    return parent_window;
}

// Adding the return type as a pointer to ImGuiIO
ImGuiIO* register_parent_glfw_window_with_imgui(GLFWwindow* pwindow, struct Logger* logger) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(pwindow, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    LOG(logger, LOG_LEVEL_INFO, "ImGui successfully initialized!\n");

    return &io; // Returning the address for io
}

int main(void) {
    // Create the logger; no file to avoid clutter during development.
    struct Logger* logger = logger_create(LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, NULL);

    // create the glfw parent window to generate the root context.
    GLFWwindow* parent_window = create_parent_glfw_window(1280, 720, "alt.cpp", NULL, logger);

    if (NULL == parent_window) {
        glfwTerminate();
        return -1;
    }

    register_parent_glfw_window_with_imgui(parent_window, logger);

    std::string prompt         = "Once upon a time";
    std::string generated_text = " a prompt generation was faked by a program named Alt.";

    logger_message(logger, LOG_LEVEL_INFO, "Generated text: %s\n", generated_text.c_str());

    while (!glfwWindowShouldClose(parent_window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, ImGui!");

        // Test OpenAI completion
        ImGui::Text("Prompt: %s", prompt.c_str());
        ImGui::Text("Completion: %s", generated_text.c_str());

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(parent_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(parent_window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(parent_window);
    glfwTerminate();
    return 0;
}
