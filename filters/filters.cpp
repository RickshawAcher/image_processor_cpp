#include "filters.h"

#include <cstdlib>

double filters::Filter::CheckBoundaries(double color) const {
    if (color < 0) {
        return 0;
    } else if (color > MAX_BITS) {
        return MAX_BITS;
    } else {
        return color;
    }
}

template <typename T>
std::vector<T> filters::Filter::ApplyMatrix(const std::vector<std::vector<T>>& matrix_filter_applier,
                                            const Image& image, size_t i, size_t j) const {
    T red = 0;
    T green = 0;
    T blue = 0;

    const int64_t image_height = static_cast<int64_t>(image.GetHeight());
    const int64_t image_width = static_cast<int64_t>(image.GetWidth());

    for (int64_t x = 0; x < matrix_filter_applier.size(); ++x) {
        for (int64_t y = 0; y < matrix_filter_applier.front().size(); ++y) {
            int64_t row_idx = static_cast<int64_t>(i) - 1 + x;
            int64_t col_idx = static_cast<int64_t>(j) - 1 + y;
            if (row_idx < 0) {
                row_idx = 0;
            }
            if (row_idx >= image_height) {
                row_idx = image_height - 1;
            }
            if (col_idx < 0) {
                col_idx = 0;
            }
            if (col_idx >= image_width) {
                col_idx = image_width - 1;
            }
            red += image.GetPixel(row_idx, col_idx).red * matrix_filter_applier[x][y];
            green += image.GetPixel(row_idx, col_idx).green * matrix_filter_applier[x][y];
            blue += image.GetPixel(row_idx, col_idx).blue * matrix_filter_applier[x][y];
        }
    }
    red = CheckBoundaries(red);
    green = CheckBoundaries(green);
    blue = CheckBoundaries(blue);
    return {blue, green, red};
}

filters::Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

Image filters::Crop::Apply(const Image& picture) const {
    const size_t crop_height = std::min(height_, picture.GetHeight());
    const size_t crop_width = std::min(width_, picture.GetWidth());
    Image result(crop_height, crop_width);
    for (size_t i = 0; i < crop_height; ++i) {
        for (size_t j = 0; j < crop_width; ++j) {
            result.SetPixel(i, j, picture.GetPixel(i, j));
        }
    }
    return result;
}

Image filters::Grayscale::Apply(const Image& photo) const {
    std::vector<std::vector<Pixel>> result(photo.GetHeight(), std::vector<Pixel>(photo.GetWidth()));
    for (size_t i = 0; i < photo.GetHeight(); ++i) {
        for (size_t j = 0; j < photo.GetWidth(); ++j) {
            double grayscale_color = photo.GetPixel(i, j).red * red_gray_coef_ +
                                     photo.GetPixel(i, j).blue * blue_gray_coef_ +
                                     photo.GetPixel(i, j).green * green_gray_coef_;
            result[i][j].SetColors(grayscale_color, grayscale_color, grayscale_color);
        }
    }
    return Image(result);
}

Image filters::Negative::Apply(const Image& picture) const {
    Image result(picture.GetHeight(), picture.GetWidth());
    for (size_t i = 0; i < picture.GetHeight(); ++i) {
        for (size_t j = 0; j < picture.GetWidth(); ++j) {
            const auto& pixel = picture.GetPixel(i, j);
            result.SetPixel(i, j, {MAX_BITS - pixel.blue, MAX_BITS - pixel.green, MAX_BITS - pixel.red});
        }
    }
    return result;
}

Image filters::Sharpening::Apply(const Image& picture) const {
    std::vector<std::vector<Pixel>> result(picture.GetHeight(), std::vector<Pixel>(picture.GetWidth()));
    const std::vector<std::vector<double>> matrix_filter_applier = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    for (size_t i = 0; i < picture.GetHeight(); ++i) {
        for (size_t j = 0; j < picture.GetWidth(); ++j) {
            std::vector<double> colors = ApplyMatrix(matrix_filter_applier, picture, i, j);
            result[i][j].blue = colors[0];
            result[i][j].green = colors[1];
            result[i][j].red = colors[2];
        }
    }
    return Image(result);
}

filters::EdgeDetection::EdgeDetection(double threshold) : threshold_(threshold) {
}

Image filters::EdgeDetection::Apply(const Image& picture) const {
    const Image gray_picture = filters::Grayscale().Apply(picture);
    const std::vector<std::vector<double>> matrix_filter_applier = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    std::vector<std::vector<Pixel>> result(gray_picture.GetHeight(), std::vector<Pixel>(gray_picture.GetWidth()));
    for (size_t i = 0; i < gray_picture.GetHeight(); ++i) {
        for (size_t j = 0; j < gray_picture.GetWidth(); ++j) {
            std::vector<double> colors = ApplyMatrix(matrix_filter_applier, gray_picture, i, j);
            result[i][j] = ((colors[0] > threshold_) ? Pixel(MAX_BITS, MAX_BITS, MAX_BITS) : Pixel(0, 0, 0));
        }
    }
    return Image(result);
}

Image filters::Prewitt::Apply(const Image& picture) const {
    const Image gray_scaled_picture = filters::Grayscale().Apply(picture);
    std::vector<std::vector<Pixel>> result(picture.GetHeight(), std::vector<Pixel>(picture.GetWidth()));
    const std::vector<std::vector<double>> kernel_x = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    const std::vector<std::vector<double>> kernel_y = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
    for (size_t i = 0; i < picture.GetHeight(); ++i) {
        for (size_t j = 0; j < picture.GetWidth(); ++j) {
            std::vector<double> colors_x = ApplyMatrix(kernel_x, gray_scaled_picture, i, j);
            std::vector<double> colors_y = ApplyMatrix(kernel_y, gray_scaled_picture, i, j);
            result[i][j].red =
                std::sqrt(std::pow(CheckBoundaries(colors_x[2]), 2) + std::pow(CheckBoundaries(colors_y[2]), 2));
            result[i][j].green =
                std::sqrt(std::pow(CheckBoundaries(colors_x[1]), 2) + std::pow(CheckBoundaries(colors_y[1]), 2));
            result[i][j].blue =
                std::sqrt(std::pow(CheckBoundaries(colors_x[0]), 2) + std::pow(CheckBoundaries(colors_y[0]), 2));
        }
    }
    return Image(result);
}

filters::Pixellate::Pixellate(int block_size) : block_size_(block_size) {
}

bool filters::Pixellate::IsPointInHexagon(size_t x, size_t y, const std::vector<std::pair<double, double>>& vertices,
                                          size_t height, size_t width) {
    bool is_in_hexagon = false;
    for (size_t i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
        if (((vertices[i].second > static_cast<double>(y)) != (vertices[j].second > static_cast<double>(y))) &&
            (static_cast<double>(x) < (vertices[j].first - vertices[i].first) *
                                              (static_cast<double>(y) - vertices[i].second) /
                                              (vertices[j].second - vertices[i].second) +
                                          vertices[i].first)) {
            is_in_hexagon = !is_in_hexagon;
        }
    }
    if (x < 0 || x >= height || y < 0 || y >= width) {
        is_in_hexagon = false;
    }
    return is_in_hexagon;
}

Image filters::Pixellate::Apply(const Image& image) const {
    Image new_image(image.GetHeight(), image.GetWidth());
    std::vector<std::vector<Pixel>> result(image.GetHeight(), std::vector<Pixel>(image.GetWidth()));

    for (size_t i = 0; i < image.GetHeight(); i += block_size_) {
        for (size_t j = 0; j < image.GetWidth(); j += block_size_) {
            double red_sum = 0;
            double green_sum = 0;
            double blue_sum = 0;
            double pixel_count = 0;

            double x_center = static_cast<double>(i) + static_cast<double>(block_size_) / 2;
            double y_center = static_cast<double>(j) + static_cast<double>(block_size_) / 2;
            double radius = block_size_ / 2;
            double apothem = radius * cos(angle_);
            std::vector<std::pair<double, double>> hexagon_vertices = {{x_center, y_center - radius},
                                                                       {x_center + apothem, y_center - radius / 2},
                                                                       {x_center + apothem, y_center + radius / 2},
                                                                       {x_center, y_center + radius},
                                                                       {x_center - apothem, y_center + radius / 2},
                                                                       {x_center - apothem, y_center - radius / 2}};

            for (size_t x = i; x < std::min(i + block_size_, image.GetHeight()); ++x) {
                for (size_t y = j; y < std::min(j + block_size_, image.GetWidth()); ++y) {
                    if (IsPointInHexagon(x, y, hexagon_vertices, image.GetHeight(), image.GetWidth())) {
                        Pixel pixel = image.GetPixel(x, y);
                        red_sum += pixel.red;
                        green_sum += pixel.green;
                        blue_sum += pixel.blue;
                        ++pixel_count;
                    }
                }
            }
            if (pixel_count > 0) {
                Pixel average_pixel;
                average_pixel.red = red_sum / pixel_count;
                average_pixel.green = green_sum / pixel_count;
                average_pixel.blue = blue_sum / pixel_count;
                for (size_t x = i; x < std::min(i + block_size_, image.GetHeight()); ++x) {
                    for (size_t y = j; y < std::min(j + block_size_, image.GetWidth()); ++y) {
                        result[x][y] = average_pixel;
                    }
                }
            }
        }
    }
    return Image(result);
}

std::unique_ptr<filters::Filter> filters::CreatePtrFilter(const DataParser::Data& argument) {
    const std::string& filter = argument.name;
    if (filter == "-crop") {
        if (argument.params.size() != 2) {
            throw std::invalid_argument("Incorrect params for -crop filter");
        }
        try {
            size_t height = std::stoi(argument.params[0]);
            size_t width = std::stoi(argument.params[1]);
            auto ptr = std::make_unique<filters::Crop>(width, height);
            return ptr;
        } catch (std::exception& e) {
            throw std::invalid_argument(e.what());
        }
    } else if (filter == "-gs") {
        if (!argument.params.empty()) {
            throw std::invalid_argument("Incorrect params for -gs filter");
        }
        try {
            auto ptr = std::make_unique<filters::Grayscale>();
            return ptr;
        } catch (std::exception& e) {
            throw std::invalid_argument(e.what());
        }
    } else if (filter == "-neg") {
        if (!argument.params.empty()) {
            throw std::invalid_argument("Incorrect params for -neg filter");
        }
        try {
            auto ptr = std::make_unique<filters::Negative>();
            return ptr;
        } catch (std::exception& e) {
            throw std::invalid_argument(e.what());
        }
    } else if (filter == "-sharp") {
        if (!argument.params.empty()) {
            throw std::invalid_argument("Incorrect params for -sharp filter");
        }
        try {
            auto ptr = std::make_unique<filters::Sharpening>();
            return ptr;
        } catch (std::exception& e) {
            throw std::invalid_argument(e.what());
        }
    } else if (filter == "-edge") {
        if (argument.params.size() != 1) {
            throw std::invalid_argument("Incorrect params for -edge filter");
        }
        try {
            double threshold = std::stod(argument.params.front());
            auto ptr = std::make_unique<filters::EdgeDetection>(threshold);
            return ptr;
        } catch (std::exception& e) {
            throw std::invalid_argument(e.what());
        }
    } else if (filter == "-prewitt") {
        if (!argument.params.empty()) {
            throw std::invalid_argument("Incorrect params for -prewitt filter");
        }
        try {
            auto ptr = std::make_unique<filters::Prewitt>();
            return ptr;
        } catch (std::exception& e) {
            throw std::invalid_argument(e.what());
        }
    } else if (filter == "-pixellate") {
        if (argument.params.size() != 1) {
            throw std::invalid_argument("Incorrect params for -pixellate filter");
        }
        try {
            int block_size = std::stoi(argument.params.front());
            auto ptr = std::make_unique<filters::Pixellate>(block_size);
            return ptr;
        } catch (std::exception& e) {
            throw std::invalid_argument(e.what());
        }
    } else {
        throw std::invalid_argument("You are wrong, bro " + filter);
    }
}
