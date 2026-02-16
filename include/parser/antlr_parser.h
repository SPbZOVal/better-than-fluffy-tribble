#pragma once

#include "iparser.h"

namespace btft::parser {

class AntlrParser final : public IParser {
public:
    AntlrParser() = default;
    [[nodiscard]] ParseResult Parse(std::string_view input) const override;
};

}  // namespace btft::parser