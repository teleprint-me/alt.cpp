/*
 * alt.cpp/source/linear.cpp
 *
 * Copyright © 2024 Austin Berrio
 */
#include <stdio.h>

double linear(double x, double m, double b) { return m * x + b; }

int linear_example() {
    double x = 3.0;
    double m = 2.0;
    double b = 1.0;
    double y = linear(x, m, b);
    printf("For x = %.2f, y = %.2f\n", x, y);
    return 0;
}
