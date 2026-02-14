#pragma once

#include <string>
#include "environment.h"

namespace btft {

class Preprocessor final {
public:
    // TODO
    // NOLINTNEXTLINE
    std::string preprocess(std::string_view raw_input, const Environment &) {
        return std::string(raw_input);
    }
};

}
