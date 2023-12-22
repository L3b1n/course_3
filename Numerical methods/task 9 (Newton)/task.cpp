#include <iostream>
#include <cmath>

double f(double x) {
    return std::pow(x, 4) -  std::pow(x, 3) + 8 * std::pow(x, 2) - x - 1;
}

double ff(double x) {
    return 4 * std::pow(x, 3) - 3 * std::pow(x, 2) + 16 * x - 1;
}

double fff(double x) {
    return 12 * std::pow(x, 2) - 6 * x + 16;
}

// Newton-Raphson method implementation
double newton_method(double initial_guess, double epsilon, int max_iterations, bool isf1) {
    double x = initial_guess;

    for (int i = 0; i < max_iterations; ++i) {
        double fx = isf1 ? f(x) : ff(x);
        double fpx = isf1 ? ff(x) : fff(x);

        if (std::abs(fpx) < 1e-10) {
            std::cerr << "Error: Division by zero in Newton method." << std::endl;
            return std::numeric_limits<double>::quiet_NaN(); // Not a number
        }

        x = x - fx / fpx;

        if (std::abs(fx) < epsilon) {
            return x;
        }
    }

    std::cerr << "Warning: Newton method did not converge within the specified number of iterations." << std::endl;
    return std::numeric_limits<double>::quiet_NaN();
}

// Bisection method implementation
double bisection_method(double a, double b, double epsilon, int max_iterations) {
    if (ff(a) * ff(b) > 0) {
        std::cerr << "Error: Bisection method requires that f(a) and f(b) have opposite signs." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double c;

    for (int i = 0; i < max_iterations; ++i) {
        c = (a + b) / 2;

        if (std::abs(ff(c)) < epsilon) {
            return c;
        }

        if (ff(c) * ff(a) < 0) {
            b = c;
        } else {
            a = c;
        }
    }

    std::cerr << "Warning: Bisection method did not converge within the specified number of iterations." << std::endl;
    return std::numeric_limits<double>::quiet_NaN();
}

int main() {
    double newton_initial_guess = -1.0; // Initial guess for the Newton method
    double bisection_a = -2;            // Lower bound for the Bisection method
    double bisection_b = 1;             // Upper bound for the Bisection method
    double epsilon = 1e-8;              // Tolerance for convergence
    int max_iterations = 1e5;           // Maximum number of iterations

    // Newton method
    double newton_root = newton_method(newton_initial_guess, epsilon, max_iterations, false);
    if (!std::isnan(newton_root)) {
        std::cout << "Approximate root using Newton method: " << newton_root << std::endl;
    } else {
        std::cerr << "Error finding root with Newton method." << std::endl;
    }

    // Bisection method
    double bisection_root = bisection_method(bisection_a, bisection_b, epsilon, max_iterations);
    if (!std::isnan(bisection_root)) {
        std::cout << "Approximate root using Bisection method: " << bisection_root << std::endl;
    } else {
        std::cerr << "Error finding root with Bisection method." << std::endl;
    }

    // Newton method f1
    newton_root = newton_method(newton_initial_guess, epsilon, max_iterations, true);
    if (!std::isnan(newton_root)) {
        std::cout << "Approximate root using Newton method for init function: " << newton_root << std::endl;
    } else {
        std::cerr << "Error finding root with Newton method." << std::endl;
    }

    return 0;
}