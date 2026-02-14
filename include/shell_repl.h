#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include "parser.h"
#include "preprocessor.h"

namespace btft {

struct ExecutionResult {
    int exit_code = 0;
    bool should_exit = false;
    std::string error_message;
};

class ShellRepl final {
public:
    ShellRepl();
    [[nodiscard]] int run() const;

private:
    static void print_prompt() {
        std::cout << kPromptPrefix << std::flush;
    }

    static bool is_blank(std::string_view s) noexcept {
        return std::ranges::all_of(s, [](char c) {
            return std::isspace(static_cast<unsigned char>(c)) != 0;
        });
    }

    [[nodiscard]] ExecutionResult process_line(
        std::string_view raw_input
    ) const;

    std::unique_ptr<parser::IParser> parser;
    std::unique_ptr<parser::Preprocessor> preprocessor;
    static constexpr std::string_view kPromptPrefix = "$ ";
};

}  // namespace btft::parser
