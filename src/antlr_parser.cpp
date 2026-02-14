#include "antlr_parser.h"

// ANTLR
#include <antlr4-runtime.h>
#include "ShellLexer.h"
#include "ShellParser.h"

// Other
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "antlr_support.h"
#include "ast_nodes.h"

namespace btft {

[[nodiscard]] std::string to_string(CommandKind kind) {
    static const std::map<CommandKind, std::string> kBuiltinName = {
        {CommandKind::kCat, "cat"},   {CommandKind::kEcho, "echo"},
        {CommandKind::kWc, "wc"},     {CommandKind::kPwd, "pwd"},
        {CommandKind::kExit, "exit"}, {CommandKind::kExternal, "<external>"}
    };

    if (const auto it = kBuiltinName.find(kind); it != kBuiltinName.end()) {
        return it->second;
    }
    throw std::runtime_error("unhandled command kind at to_string!");
}

namespace {

CommandKind resolve_command_kind(std::string_view name) {
    static const std::map<std::string_view, CommandKind> kBuiltinName = {
        {"cat", CommandKind::kCat},   {"echo", CommandKind::kEcho},
        {"wc", CommandKind::kWc},     {"pwd", CommandKind::kPwd},
        {"exit", CommandKind::kExit},
    };

    if (const auto it = kBuiltinName.find(name); it != kBuiltinName.end()) {
        return it->second;
    }
    return CommandKind::kExternal;
}

// TODO: refactor once pipes will be implemented
PipelineNode build_pipeline_from_words(const std::vector<std::string> &words) {
    PipelineNode pipeline;
    if (words.empty()) {
        return pipeline;
    }

    std::string name = words.front();
    auto args = std::vector(words.begin() + 1, words.end());

    pipeline.add_command(CommandNode(
        std::move(name), resolve_command_kind(words.front()), std::move(args)
    ));
    return pipeline;
}

}  // namespace

ParseResult AntlrParser::parse(std::string_view input) const {
    antlr4::ANTLRInputStream stream(input);
    ShellLexer lexer(&stream);
    antlr4::CommonTokenStream tokens(&lexer);

    CountingErrorListener error_listener;
    lexer.removeErrorListeners();
    lexer.addErrorListener(&error_listener);

    ShellParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&error_listener);

    parser.line();

    if (error_listener.get_errors() != 0) {
        return ParseResult::error("syntax error");
    }

    const std::vector<std::string> words = collect_words(tokens);
    PipelineNode pipeline = build_pipeline_from_words(words);
    return ParseResult::ok(std::move(pipeline));
}

}  // namespace btft
