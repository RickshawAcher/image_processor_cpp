#pragma once

#include <vector>

namespace input_and_output {
const int SIZE_OF_BYTE = 8;
const int MASKING_VARIABLE = 255;
const int PLANES = 1;
const int BITS_PER_PIXEL = 24;
const int NUMBER_OF_BITS_PER_PIXEL_POS = 14;
const int PLANES_POS = 12;
const int BYTES_PER_PIXEL = 3;
const std::vector<int> KINDS_OF_BYTES_VOLUME = {8, 16, 24};
const std::vector<char> FORMAT_BYTES_VALUE = {'B', 'M'};
const int FILE_HEADER_SIZE = 14;
const int INF_HEADER_SIZE = 40;
const int PADDING_ALIGNMENT = 4;
const int FORMAT_SIZE_FIRST_POS = 0;
const int FORMAT_SIZE_SECOND_POS = 1;
const int HEADER_FILE_SIZE_GAP = 2;
const int HEADERS_SUM_POS = 10;
const int HEADER_WIDTH_GAP = 4;
const int HEADER_HEIGHT_GAP = 8;
const int INFORMATION_HEADER_SIZE_POS = 0;
}  // namespace input_and_output
