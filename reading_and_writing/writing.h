#include <fstream>
#include "../BMP_image/bmp_image.h"
#include <vector>
#include <cstddef>

namespace input_and_output {
class Writer {
public:
    explicit Writer(std::string filename);
    void WriteFile(const Image &image);

private:
    void Header(unsigned char *file_header, size_t file_size);
    void InformHeader(unsigned char *information_header, size_t height, size_t width);

    static void NumberToBytesUint32(uint32_t number, unsigned char *bytes);
    static void NumberToBytesUint16(uint16_t number, unsigned char *bytes);

    const double max_color_ = 255;

private:
    std::string filename_;
};
}  // namespace input_and_output
