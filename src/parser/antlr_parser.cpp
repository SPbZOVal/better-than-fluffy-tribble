#include "parser/antlr_parser.h"

// ANTLR
#include <antlr4-runtime.h>
#include "ShellBaseVisitor.h"
#include "ShellLexer.h"
#include "ShellParser.h"

// Other
#include <any>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "common.h"
#include "parser/antlr_support.h"

namespace btft::parser {

namespace {

class AstBuilder final : public ShellBaseVisitor {
public:
    interpreter::PipelineNode Build(ShellParser::LineContext *line_ctx) {
        using interpreter::PipelineNode;
        if (line_ctx == nullptr || line_ctx->stmt() == nullptr) {
            return PipelineNode{};
        }

        const std::any res_any = visitStmt(line_ctx->stmt());
        return std::any_cast<PipelineNode>(res_any);
    }

private:
    // NOLINTNEXTLINE
    std::any visitStmt(ShellParser::StmtContext *ctx) override {
        using interpreter::CommandNode;
        using interpreter::PipelineNode;

        PipelineNode pipeline;

        std::vector<std::string> assigns;
        assigns.reserve(ctx->assignment().size());

        for (ShellParser::AssignmentContext *a : ctx->assignment()) {
            assigns.push_back(ParseAssignment(a));
        }

        if (!assigns.empty()) {
            pipeline.AddCommand(
                CommandNode(std::string(kAssignCommandName), std::move(assigns))
            );
        }

        if (ctx->command() != nullptr) {
            pipeline.AddCommand(ParseCommand(ctx->command()));
        }

        return pipeline;
    }

    static std::string ParseAssignment(ShellParser::AssignmentContext *ctx) {
        const std::string name = ctx->NAME()->getText();

        const ShellParser::WordContext *word_ctx = ctx->value()->word();
        const antlr4::Token *t = word_ctx->getStart();
        const std::string value = DecodeWordToken(*t);

        return name + "=" + value;
    }

    static interpreter::CommandNode ParseCommand(
        ShellParser::CommandContext *ctx
    ) {
        using interpreter::CommandNode;
        std::vector<std::string> words;
        words.reserve(ctx->word().size());

        for (const ShellParser::WordContext *w : ctx->word()) {
            const antlr4::Token *t = w->getStart();
            words.push_back(DecodeWordToken(*t));
        }

        std::string name = std::move(words.front());
        words.erase(words.begin());

        return {std::move(name), std::move(words)};
    }

    static constexpr std::string_view kAssignCommandName = "__assign__";
};

}  // namespace

ParseResult AntlrParser::Parse(std::string_view input) const {
    using interpreter::PipelineNode;

    antlr4::ANTLRInputStream stream(input);
    ShellLexer lexer(&stream);
    antlr4::CommonTokenStream tokens(&lexer);

    ParserErrorListener error_listener;
    lexer.removeErrorListeners();
    lexer.addErrorListener(&error_listener);

    ShellParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&error_listener);

    ShellParser::LineContext *tree = parser.line();

    if (const auto pos = error_listener.GetErrorCharPosition(); pos) {
        return ParseResult::Error(
            "syntax error at " + std::to_string(*pos) + " symbol"
        );
    }

    AstBuilder builder;
    PipelineNode pipeline = builder.Build(tree);
    return ParseResult::Ok(std::move(pipeline));
}

}  // namespace btft::parser
