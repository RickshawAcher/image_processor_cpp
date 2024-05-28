add_executable(
    image_processor
    image_processor.cpp
        data_parser/parser.h data_parser/parser.cpp BMP_image/bmp_image.h BMP_image/bmp_image.cpp pixel/pixel.h pixel/pixel.cpp reading_and_writing/reading.h reading_and_writing/reading.cpp reading_and_writing/writing.h reading_and_writing/writing.cpp reading_and_writing/writing.cpp filters/filters.h filters/filters.cpp reading_and_writing/constants.h)
