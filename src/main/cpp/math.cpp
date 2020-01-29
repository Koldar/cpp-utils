#include "math.hpp"

namespace cpp_utils {

    double getNormal(double x, double mean, double stddev) {
        return (1/(stddev * sqrt(2 * pi()))) * exp(-0.5 * pow((x - mean)/stddev, 2));
    }

    double getGaussian(double x, double stddev) {
        return exp(-0.5 * pow((x)/stddev, 2));
    }

    double getGaussian(double minY, double maxY, double x, double stddev) {
        return minY + (maxY - minY) * getGaussian(x, stddev);
    }

    double getCenteredGaussian(double x, double stddev, double minX, double maxX, double minY, double maxY) {
        auto width = maxX - minX;
        return getGaussian(minY, maxY, x - (width * 0.5), ( 2 * stddev) / ( width));
    }

    double getLeftGaussian(double x, double stddev, double minX, double maxX, double minY, double maxY) {
        auto width = maxX - minX;
        return getGaussian(minY, maxY, x - maxX, ( 2 * stddev) / ( width));
    }

    double getRightGaussian(double x, double stddev, double minX, double maxX, double minY, double maxY) {
        auto width = maxX - minX;
        return getGaussian(minY, maxY, x - minX, ( 2 * stddev) / ( width));
    }

    double pow10(int a) {
        static double pow10[] = {
            1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1,
            1,
            1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 1e+6
        };
        return pow10[6 + a]; 
    }

}