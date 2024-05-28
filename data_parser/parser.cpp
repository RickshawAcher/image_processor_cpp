#include "parser.h"

void DataParser::Data::Clear() {
    name.clear();
    params.clear();
}

bool DataParser::Data::Empty() const {
    return name.empty() && params.empty();
}

std::vector<DataParser::Data> DataParser::Parse(int& argc, char* argv[]) {
    std::vector<DataParser::Data> result;
    DataParser::Data current;
    for (size_t i = 1; i < argc; ++i) {
        std::string arg = static_cast<std::string>(argv[i]);
        if (i <= 2) {
            current.name = arg;
            result.push_back(current);
            current.Clear();
        } else {
            if (arg.front() == '-') {
                if (!current.Empty()) {
                    result.push_back(current);
                    current.Clear();
                }
                current.name = arg;
            } else {
                current.params.push_back(arg);
            }
        }
    }
    if (!current.Empty()) {
        result.push_back(current);
    }
    if (result.size() <= 2) {
        throw std::invalid_argument("You have not entered enough parameters.");
    }
    return result;
}
