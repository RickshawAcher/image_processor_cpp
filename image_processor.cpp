#include "data_parser/parser.h"
#include "BMP_image/bmp_image.h"
#include "filters/filters.h"
#include "reading_and_writing/reading.h"
#include "reading_and_writing/writing.h"

#include <iostream>

Image Input(const std::string& input_file) {
    input_and_output::Reader reader(input_file);
    Image image = reader.ReadFile();
    return image;
}

void Output(const std::string& output_file, const Image& image) {
    input_and_output::Writer writer(output_file);
    writer.WriteFile(image);
}

int main(int argc, char* argv[]) {
    try {
        std::vector<DataParser::Data> arguments = DataParser::Parse(argc, argv);
        Image image = Input(arguments[0].name);
        for (size_t i = 2; i < arguments.size(); ++i) {
            image = filters::CreatePtrFilter(arguments[i])->Apply(image);
        }
        Output(arguments[1].name, image);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
