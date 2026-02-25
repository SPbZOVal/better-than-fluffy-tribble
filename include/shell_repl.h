#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include "common.h"
#include "parser/iparser.h"

namespace btft {

class ShellRepl final {
public:
    ShellRepl();
    [[nodiscard]] int Run() const;

private:
    static void PrintPrompt() {
        std::cout << kPromptPrefix << std::flush;
    }

    static bool IsBlank(std::string_view s) noexcept {
        return std::ranges::all_of(s, [](char c) {
            return std::isspace(static_cast<unsigned char>(c)) != 0;
        });
    }

    [[nodiscard]] interpreter::ExecutionResult ProcessLine(
        std::string_view raw_input
    ) const;

    std::unique_ptr<parser::IParser> parser;
    static constexpr std::string_view kPromptPrefix = ">";
};

}  // namespace btft
