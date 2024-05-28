#include "writing.h"
#include "reading.h"

input_and_output::Writer::Writer(std::string filename) : filename_(filename) {
}

void input_and_output::Writer::NumberToBytesUint32(uint32_t number, unsigned char *bytes) {
    *bytes = number & MASKING_VARIABLE;
    for (size_t i = 1; i < 4; ++i) {
        bytes[i] = (number >> (SIZE_OF_BYTE * i)) & MASKING_VARIABLE;
    }
}

void input_and_output::Writer::NumberToBytesUint16(uint16_t number, unsigned char *bytes) {
    bytes[0] = number & MASKING_VARIABLE;
    bytes[1] = (number >> SIZE_OF_BYTE) & MASKING_VARIABLE;
}

void input_and_output::Writer::Header(unsigned char *file_header, size_t file_size) {
    file_header[FORMAT_SIZE_FIRST_POS] = FORMAT_BYTES_VALUE[0];
    file_header[FORMAT_SIZE_SECOND_POS] = FORMAT_BYTES_VALUE[1];
    NumberToBytesUint32(file_size, file_header + HEADER_FILE_SIZE_GAP);
    NumberToBytesUint32(FILE_HEADER_SIZE + INF_HEADER_SIZE, file_header + HEADERS_SUM_POS);
}

void input_and_output::Writer::InformHeader(unsigned char *inf_header, size_t height, size_t width) {
    NumberToBytesUint32(INF_HEADER_SIZE, inf_header + INFORMATION_HEADER_SIZE_POS);
    NumberToBytesUint32(width, inf_header + HEADER_WIDTH_GAP);
    NumberToBytesUint32(height, inf_header + HEADER_HEIGHT_GAP);
    NumberToBytesUint16(PLANES, inf_header + PLANES_POS);
    NumberToBytesUint16(BITS_PER_PIXEL, inf_header + NUMBER_OF_BITS_PER_PIXEL_POS);
}

void input_and_output::Writer::WriteFile(const Image &image_file) {
    std::ofstream output_file;
    output_file.open(filename_, std::ios::out | std::ios::binary);
    if (!output_file.is_open()) {
        throw std::invalid_argument("Can't open file: " + filename_);
    }

    const size_t file_size = FILE_HEADER_SIZE + INF_HEADER_SIZE + image_file.GetWidth() * image_file.GetHeight() * 3 +
                             GetPadding(image_file.GetWidth()) * image_file.GetWidth();

    unsigned char file_header[FILE_HEADER_SIZE];
    for (size_t i = 0; i < FILE_HEADER_SIZE; ++i) {
        file_header[i] = 0;
    }
    Header(file_header, file_size);

    unsigned char inf_header[INF_HEADER_SIZE];
    for (size_t i = 0; i < INF_HEADER_SIZE; ++i) {
        inf_header[i] = 0;
    }

    InformHeader(inf_header, image_file.GetHeight(), image_file.GetWidth());
    try {
        output_file.write(reinterpret_cast<char *>(file_header), FILE_HEADER_SIZE);
        output_file.write(reinterpret_cast<char *>(inf_header), INF_HEADER_SIZE);

        unsigned char empty_bmp[3] = {0, 0, 0};
        for (size_t x = 0; x < image_file.GetHeight(); ++x) {
            for (size_t y = 0; y < image_file.GetWidth(); ++y) {
                auto blue = static_cast<unsigned char>(
                    static_cast<uint8_t>(image_file.GetPixel(image_file.GetHeight() - x - 1, y).blue * max_color_));
                auto green = static_cast<unsigned char>(
                    static_cast<uint8_t>(image_file.GetPixel(image_file.GetHeight() - x - 1, y).green * max_color_));
                auto red = static_cast<unsigned char>(
                    static_cast<uint8_t>(image_file.GetPixel(image_file.GetHeight() - x - 1, y).red * max_color_));
                unsigned char pixel[] = {blue, green, red};
                output_file.write(reinterpret_cast<char *>(pixel), BYTES_PER_PIXEL);
            }
            output_file.write(reinterpret_cast<char *>(empty_bmp), GetPadding(image_file.GetWidth()));
        }
        output_file.close();
    } catch (std::exception &exc) {
        throw static_cast<std::string>(exc.what());
    }
}
