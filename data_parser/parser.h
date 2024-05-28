#pragma once

#include <string>
#include <vector>
#include <stdexcept>

class DataParser {
public:
    struct Data {
        std::string name;
        std::vector<std::string> params;
        bool Empty() const;
        void Clear();
    };

    static std::vector<Data> Parse(int& argc, char* argv[]);
};
