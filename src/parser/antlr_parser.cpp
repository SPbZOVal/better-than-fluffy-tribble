#include "parser/antlr_parser.h"

// ANTLR
#include <antlr4-runtime.h>
#include "ShellBaseVisitor.h"
#include "ShellLexer.h"
#include "ShellParser.h"

// Other
#include <environment.h>
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

        const bool has_commands = ctx->pipe() != nullptr;

        if (!ctx->assignment().empty()) {
            const bool make_global = !has_commands;
            ApplyAssignments(ctx->assignment(), make_global);
        }

        PipelineNode pipeline;
        if (has_commands) {
            for (const auto &cmd : ParsePipe(ctx->pipe())) {
                pipeline.AddCommand(cmd);
            }
        }

        return pipeline;
    }

    static void ApplyAssignments(
        const std::vector<ShellParser::AssignmentContext *> &assignments,
        bool make_global
    ) {
        auto &env = Environment::GetInstance();

        for (ShellParser::AssignmentContext *a : assignments) {
            const std::string name = a->NAME()->getText();
            const auto [text, allow_expansion] =
                MakeSegmentFromWord(a->value()->word());

            const std::string value =
                allow_expansion ? ExpandVars(text, env) : text;

            if (make_global) {
                env.SetGlobal(name, value);
            } else {
                env.SetLocal(name, value);
            }
        }
    }

    static std::string ExpandVars(std::string_view s, const Environment &env) {
        std::string out;
        out.reserve(s.size());

        for (std::size_t i = 0; i < s.size(); ++i) {
            if (const char c = s[i]; c != '$') {
                out.push_back(c);
                continue;
            }

            if (i + 1 >= s.size() || !interpreter::IsVarStart(s[i + 1])) {
                out.push_back('$');
                continue;
            }

            std::size_t j = i + 1;
            while (j < s.size() && interpreter::IsVarChar(s[j])) {
                ++j;
            }

            const std::string name(s.substr(i + 1, j - (i + 1)));
            if (const auto val = env.GetVar(name); val.has_value()) {
                out += *val;
            }

            i = j - 1;
        }

        return out;
    }

    static std::vector<interpreter::CommandNode> ParsePipe(
        ShellParser::PipeContext *ctx
    ) {
        std::vector<interpreter::CommandNode> out;
        out.reserve(ctx->command().size());

        for (ShellParser::CommandContext *c : ctx->command()) {
            out.emplace_back(ParseCommand(c));
        }

        return out;
    }

    static interpreter::ArgSegment MakeSegmentFromWord(
        const ShellParser::WordContext *word_ctx
    ) {
        const antlr4::Token *t = word_ctx->getStart();
        const std::string text = DecodeWordToken(*t);

        if (t->getType() == ShellLexer::SQ_STRING) {
            return interpreter::ArgSegment{
                .text = text, .allow_expansion = false
            };
        }

        return interpreter::ArgSegment{.text = text, .allow_expansion = true};
    }

    static interpreter::CommandNode ParseCommand(
        ShellParser::CommandContext *ctx
    ) {
        const auto &ws = ctx->word();
        std::vector<interpreter::ArgToken> glued_args;
        glued_args.reserve(ws.size());

        interpreter::ArgToken current;
        const antlr4::Token *prev_stop = nullptr;

        for (const auto *w : ws) {
            const antlr4::Token *start = w->getStart();
            const antlr4::Token *stop = w->getStop();

            const bool adjacent =
                (prev_stop != nullptr) &&
                (prev_stop->getStopIndex() + 1 == start->getStartIndex());

            if (!adjacent) {
                if (!current.Empty()) {
                    glued_args.push_back(std::move(current));
                    current = interpreter::ArgToken{};
                }
            }

            current.segments.push_back(MakeSegmentFromWord(w));
            prev_stop = stop;
        }

        if (!current.Empty()) {
            glued_args.push_back(std::move(current));
        }

        interpreter::ArgToken name = std::move(glued_args.front());
        glued_args.erase(glued_args.begin());

        return {std::move(name), std::move(glued_args)};
    }
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
