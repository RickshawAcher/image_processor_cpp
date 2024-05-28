#include "bmp_image.h"

#include <stdexcept>

Image::Image(const std::vector<Row>& data)
    : data_(data), height_(data.size()), width_(data.empty() ? 0 : data.front().size()) {
}

Image::Image(size_t height, size_t width) : data_(height, Row(width)), height_(height), width_(width) {
}

Image::Image(const Image& other) : data_(other.data_), height_(other.height_), width_(other.width_) {
}

void Image::CheckHeightAndWidth(size_t x, size_t y) const {
    if (x >= height_ || y >= width_) {
        throw std::invalid_argument("Coordinates are out of range during GetPixel or SetPixel");
    }
}

void Image::SetPixel(size_t x, size_t y, Pixel pixel) {
    CheckHeightAndWidth(x, y);
    data_[x][y] = pixel;
}

const Pixel& Image::GetPixel(size_t x, size_t y) const {
    CheckHeightAndWidth(x, y);
    return data_[x][y];
}

size_t Image::GetHeight() const {
    return height_;
}

size_t Image::GetWidth() const {
    return width_;
}
