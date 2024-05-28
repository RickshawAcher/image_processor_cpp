#pragma once
#include "../pixel/pixel.h"

#include <cstdint>
#include <cstddef>
#include <vector>

class Image {
public:
    using Row = std::vector<Pixel>;
    Image() = default;
    explicit Image(const std::vector<Row>& data);
    Image(size_t height, size_t width);
    Image(const Image& other);
    ~Image() = default;

    void SetPixel(size_t x, size_t y, Pixel pixel);
    const Pixel& GetPixel(size_t x, size_t y) const;

    size_t GetHeight() const;
    size_t GetWidth() const;

private:
    void CheckHeightAndWidth(size_t x, size_t y) const;
    std::vector<Row> data_;
    size_t height_ = 0;
    size_t width_ = 0;
};
