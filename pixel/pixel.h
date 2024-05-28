#pragma once

#include <cstdint>

struct Pixel {
    Pixel() = default;
    Pixel(double blue, double green, double red);
    Pixel& operator=(const Pixel& pixel);

    double red = 0;
    double green = 0;
    double blue = 0;

    void SetColors(uint8_t pixel_blue, uint8_t pixel_green, uint8_t pixel_red);
    void SetColors(double pixel_blue, double pixel_green, double pixel_red);
    const double MAX_COLOR = 255;
};
