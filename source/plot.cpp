/*
 * Copyright © 2024 Austin Berrio
 *
 * alt.cpp/source/plot.cpp
 */

#include "../imgui/imgui.h"
#include "../implot/implot.h"

#include <stdio.h>

double linear(double x, double m, double b) {
    return m * x + b;
}

int main() {
    // Initialize ImGUI context and style
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Create main window
    bool open = true;
    while (open) {
        ImGui::Begin("Linear Equation", &open);

        // Input fields for slope and intercept
        static double m = 2.0;
        static double b = 1.0;
        ImGui::InputDouble("Slope (m)", &m, 0.1);
        ImGui::InputDouble("Intercept (b)", &b, 0.1);

        // Plot the line using ImPlot
        ImPlot::CreateContext();
        ImVec2 plot_size = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};
        ImGui::SetNextItemWidth(plot_size.x);
        // no instance of overloaded function "ImPlot::BeginPlot" matches the argument list (E304)
        // argument types are: (const char [16], long, int, ImPlotFlags_)
        if (ImPlot::BeginPlot("Linear Equation", NULL, 0, ImPlotFlags_NoMenus)) {
            double xmin = -10.0;
            double xmax = 10.0;
            double ymin = linear(xmin, m, b);
            double ymax = linear(xmax, m, b);
            ImPlot::SetupAxes(
                "X",
                "Y",
                // No suitable conversion function from ImVec4 to ImPlotAxisFlags exists (E413)
                ImGui::GetStyle().Colors[ImGuiCol_Text], // ImPlotAxisFlags x_flags
                ImGui::GetStyle().Colors[ImGuiCol_Text]  // ImPlotAxisFlags y_flags
            );
            // no instance of overloaded function "ImPlot::PlotLine" matches the argument list
            // (E304) argument types are: (const char [16], lambda [](double x, void *data)->double,
            // double *, int, double, double)
            ImPlot::PlotLine(
                "Linear Equation",
                [](double x, void* data) { return linear(x, *(double*) data, 0); },
                &m,
                1,
                ymin,
                ymax
            );
            ImPlot::EndPlot();
        }

        // Submit button to calculate and display result
        if (ImGui::Button("Calculate")) {
            double x = 3.0;
            double y = linear(x, m, b);
            printf("For x = %.2f, y = %.2f\n", x, y);
        }

        // Close main window
        ImGui::End();

        // Render ImGUI frame
        ImGui::Render();
    }

    return 0;
}
