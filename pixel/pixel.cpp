#include "pixel.h"

Pixel::Pixel(double blue, double green, double red) {
    this->blue = blue;
    this->green = green;
    this->red = red;
}

void Pixel::SetColors(uint8_t pixel_blue, uint8_t pixel_green, uint8_t pixel_red) {
    blue = static_cast<double>(pixel_blue) / MAX_COLOR;
    green = static_cast<double>(pixel_green) / MAX_COLOR;
    red = static_cast<double>(pixel_red) / MAX_COLOR;
}

void Pixel::SetColors(double pixel_blue, double pixel_green, double pixel_red) {
    blue = static_cast<double>(pixel_blue);
    green = static_cast<double>(pixel_green);
    red = static_cast<double>(pixel_red);
}

Pixel& Pixel::operator=(const Pixel& pixel) {
    blue = pixel.blue;
    green = pixel.green;
    red = pixel.red;
    return *this;
}
