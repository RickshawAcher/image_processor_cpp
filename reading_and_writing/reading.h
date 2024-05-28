#pragma once

#include "../BMP_image/bmp_image.h"
#include <string>
#include "constants.h"

namespace input_and_output {
class Reader {
public:
    explicit Reader(const std::string& file_name);
    Image ReadFile();

private:
    std::string file_;
};

const int GetPadding(size_t width);

uint32_t LittleEndianBytesToUint32(unsigned char* bytes);

}  // namespace input_and_output
