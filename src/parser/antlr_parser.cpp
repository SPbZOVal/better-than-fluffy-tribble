#include "parser/antlr_parser.h"

// ANTLR
#include <antlr4-runtime.h>
#include "ShellLexer.h"
#include "ShellParser.h"

// Other
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "common.h"
#include "parser/antlr_support.h"

namespace btft::parser {

namespace {

// TODO: refactor once pipes will be implemented
interpreter::PipelineNode build_pipeline_from_words(
    const std::vector<std::string> &words
) {
    interpreter::PipelineNode pipeline;
    if (words.empty()) {
        return pipeline;
    }

    std::string name = words.front();
    auto args = std::vector(words.begin() + 1, words.end());

    pipeline.add_command(
        interpreter::CommandNode(std::move(name), std::move(args))
    );
    return pipeline;
}

}  // namespace

ParseResult AntlrParser::parse(std::string_view input) const {
    antlr4::ANTLRInputStream stream(input);
    ShellLexer lexer(&stream);
    antlr4::CommonTokenStream tokens(&lexer);

    ParserErrorListener error_listener;
    lexer.removeErrorListeners();
    lexer.addErrorListener(&error_listener);

    ShellParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&error_listener);

    parser.line();

    if (const auto pos = error_listener.get_error_char_position(); pos) {
        return ParseResult::error(
            "syntax error at " + std::to_string(*pos) + " symbol"
        );
    }

    const std::vector<std::string> words = collect_words(tokens);
    interpreter::PipelineNode pipeline = build_pipeline_from_words(words);
    return ParseResult::ok(std::move(pipeline));
}

}  // namespace btft::parser
