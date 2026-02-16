#pragma once

#include <string>

namespace btft::parser {

class Preprocessor final {
public:
    // TODO
    // NOLINTNEXTLINE
    [[nodiscard]] std::string Preprocess(std::string_view raw_input) {
        return std::string(raw_input);
    }
};

}  // namespace btft::parser
