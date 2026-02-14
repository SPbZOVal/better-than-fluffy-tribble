#pragma once

#include "parser.h"

namespace btft::parser {

class AntlrParser final : public IParser {
public:
    AntlrParser() = default;
    [[nodiscard]] ParseResult parse(std::string_view input) const override;
};

}  // namespace btft::parser