#include "parser/antlr_support.h"
#include <string_view>
#include "ShellLexer.h"

namespace btft::parser {

namespace {

template <char Quote>
    requires(Quote == '\'' || Quote == '"')
[[nodiscard]] std::string UnescapeQuoted(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s.at(i) == '\\' && i + 1 < s.size()) {
            if (const char next = s.at(i + 1); next == Quote) {
                out.push_back(next);
                ++i;
                continue;
            }
        }
        out.push_back(s.at(i));
    }
    return out;
}

}  // namespace

std::string DecodeWordToken(const antlr4::Token &token) {
    std::string text = token.getText();
    auto good_quoted = [&text](char q) -> bool {
        return text.size() >= 2 && text.front() == q && text.back() == q;
    };

    switch (token.getType()) {
        case ShellLexer::SQ_STRING:
            if (good_quoted('\'')) {
                return UnescapeQuoted<'\''>(
                    std::string_view(text).substr(1, text.size() - 2)
                );
            }
            break;

        case ShellLexer::DQ_STRING:
            if (good_quoted('"')) {
                return UnescapeQuoted<'"'>(
                    std::string_view(text).substr(1, text.size() - 2)
                );
            }
            break;

        default:
            break;
    }

    return text;
}

std::optional<std::size_t> ParserErrorListener::GetErrorCharPosition(
) const noexcept {
    return error_char_position;
}

void ParserErrorListener::syntaxError(
    antlr4::Recognizer *,
    antlr4::Token *,
    std::size_t,
    std::size_t char_position_in_line,
    const std::string &,
    std::exception_ptr
) {
    error_char_position = char_position_in_line;
}

}  // namespace btft::parser
