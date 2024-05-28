#pragma once

#include "../BMP_image/bmp_image.h"
#include "../data_parser/parser.h"

#include <memory>
#include <cmath>

namespace filters {

class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    virtual Image Apply(const Image& picture) const = 0;

protected:
    double CheckBoundaries(double color) const;
    const double MAX_BITS = 1.0;
    template <typename T>
    std::vector<T> ApplyMatrix(const std::vector<std::vector<T>>& matrix_filter_applier, const Image& image, size_t i,
                               size_t j) const;
};

class Crop : public Filter {
public:
    Crop(size_t width, size_t height);
    Image Apply(const Image& picture) const override;

private:
    size_t width_;
    size_t height_;
};

class Grayscale : public Filter {
public:
    Image Apply(const Image& picture) const override;

private:
    const double blue_gray_coef_ = 0.114;
    const double green_gray_coef_ = 0.587;
    const double red_gray_coef_ = 0.299;
};

class Negative : public Filter {
public:
    Image Apply(const Image& picture) const override;
};

class Sharpening : public Filter {
public:
    Image Apply(const Image& picture) const override;
};

class EdgeDetection : public Filter {
public:
    explicit EdgeDetection(double threshold);
    Image Apply(const Image& picture) const override;

private:
    double threshold_;
};

class Prewitt : public Filter {
public:
    Image Apply(const Image& picture) const override;
};

class Pixellate : public Filter {
public:
    explicit Pixellate(int block_size);
    Image Apply(const Image& image) const override;

private:
    static bool IsPointInHexagon(size_t x, size_t y, const std::vector<std::pair<double, double>>& vertices,
                                 size_t height, size_t width);
    int block_size_;
    const double angle_ = M_PI / 6;
};

std::unique_ptr<filters::Filter> CreatePtrFilter(const DataParser::Data& argument);

}  // namespace filters
