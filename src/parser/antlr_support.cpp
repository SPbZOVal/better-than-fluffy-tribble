#include "parser/antlr_support.h"
#include <string_view>
#include "ShellLexer.h"

namespace btft::parser {

namespace {

template <char quote>
    requires(quote == '\'' || quote == '"')
[[nodiscard]] std::string unescape_quoted(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s.at(i) == '\\' && i + 1 < s.size()) {
            if (const char next = s.at(i + 1); next == quote) {
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

std::string decode_word_token(const antlr4::Token &token) {
    std::string text = token.getText();
    auto good_quoted = [&text](char q) -> bool {
        return text.size() >= 2 && text.front() == q && text.back() == q;
    };

    switch (token.getType()) {
        case ShellLexer::SQ_STRING:
            if (good_quoted('\'')) {
                return unescape_quoted<'\''>(
                    std::string_view(text).substr(1, text.size() - 2)
                );
            }
            break;

        case ShellLexer::DQ_STRING:
            if (good_quoted('"')) {
                return unescape_quoted<'"'>(
                    std::string_view(text).substr(1, text.size() - 2)
                );
            }
            break;

        default:
            break;
    }

    return text;
}

std::optional<std::size_t>
ParserErrorListener::get_error_char_position() const noexcept {
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

std::vector<std::string> collect_words(antlr4::CommonTokenStream &tokens) {
    std::vector<std::string> out;
    tokens.fill();

    for (const antlr4::Token *t : tokens.getTokens()) {
        if (t == nullptr) {
            continue;
        }

        if (const std::size_t type = t->getType();
            type == ShellLexer::WORD || type == ShellLexer::SQ_STRING ||
            type == ShellLexer::DQ_STRING) {
            out.push_back(decode_word_token(*t));
        }
    }

    return out;
}

}  // namespace btft::parser
