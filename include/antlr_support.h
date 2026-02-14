#pragma once

#include <antlr4-runtime.h>
#include <optional>
#include <string>
#include <vector>

namespace btft::parser {

class ParserErrorListener final : public antlr4::BaseErrorListener {
public:
    [[nodiscard]] std::optional<std::size_t>
    get_error_char_position() const noexcept;

private:
    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offending_symbol,
        std::size_t line,
        std::size_t char_position_in_line,
        const std::string &msg,
        std::exception_ptr e
    ) override;

    std::optional<std::size_t> error_char_position;
};

std::vector<std::string> collect_words(antlr4::CommonTokenStream &tokens);

}  // namespace btft::parser
