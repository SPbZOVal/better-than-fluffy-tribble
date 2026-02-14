#pragma once

#include <antlr4-runtime.h>
#include <string>
#include <vector>

namespace btft {

class CountingErrorListener final : public antlr4::BaseErrorListener {
public:
    [[nodiscard]] int get_errors() const;

private:
    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offending_symbol,
        size_t line,
        size_t char_position_in_line,
        const std::string &msg,
        std::exception_ptr e
    ) override;

    int errors = 0;
};

std::vector<std::string> collect_words(antlr4::CommonTokenStream &tokens);

}  // namespace btft
