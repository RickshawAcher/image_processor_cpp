#include "reading.h"

#include <cerrno>
#include <fstream>
#include <vector>
#include <iostream>

uint32_t input_and_output::LittleEndianBytesToUint32(unsigned char *bytes) {
    uint32_t number = 0;
    for (int i = 0; i < 4; ++i) {
        number += bytes[i] << (SIZE_OF_BYTE * i);
    }
    return number;
}

input_and_output::Reader::Reader(const std::string &file_name) : file_(file_name) {
}

const int input_and_output::GetPadding(size_t width) {
    if ((width * BYTES_PER_PIXEL) % PADDING_ALIGNMENT == 0) {
        return 0;
    } else {
        return static_cast<int>(PADDING_ALIGNMENT - (width * BYTES_PER_PIXEL) % PADDING_ALIGNMENT);
    }
}

Image input_and_output::Reader::ReadFile() {
    unsigned char file_header[FILE_HEADER_SIZE];
    unsigned char inf_header[INF_HEADER_SIZE];

    std::ifstream image_file;
    image_file.open(file_, std::ios::in | std::ios::binary);
    if (!image_file.is_open()) {
        throw std::invalid_argument("Cannot open the file: " + static_cast<std::string>(file_));
    }
    if (errno == EACCES) {
        throw std::invalid_argument("No access to the file: " + static_cast<std::string>(file_));
    }
    image_file.read(reinterpret_cast<char *>(file_header), FILE_HEADER_SIZE);
    if (file_header[0] != FORMAT_BYTES_VALUE[0] || file_header[1] != FORMAT_BYTES_VALUE[1]) {
        throw std::invalid_argument("Incorrect file format: " + static_cast<std::string>(file_));
    }
    try {
        image_file.read(reinterpret_cast<char *>(inf_header), INF_HEADER_SIZE);
        uint32_t width = LittleEndianBytesToUint32(inf_header + HEADER_WIDTH_GAP);
        uint32_t height = LittleEndianBytesToUint32(inf_header + HEADER_HEIGHT_GAP);
        Image image(height, width);
        std::vector<std::vector<Pixel>> image_data(height, std::vector<Pixel>(width));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char pixel[BYTES_PER_PIXEL];
                image_file.read(reinterpret_cast<char *>(pixel), BYTES_PER_PIXEL);
                image_data[height - i - 1][j].SetColors(pixel[0], pixel[1], pixel[2]);
            }
            image_file.ignore(GetPadding(width));
        }
        image = Image(image_data);
        image_file.close();
        return image;

    } catch (std::exception &exc) {
        throw "Cannot read the file: " + static_cast<std::string>(exc.what());
    }
}
