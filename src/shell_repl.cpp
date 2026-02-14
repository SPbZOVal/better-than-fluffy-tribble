#include "shell_repl.h"

// ANTLR
#include <antlr4-runtime.h>

// Other
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "antlr_parser.h"
#include "environment.h"
#include "preprocessor.h"

namespace btft {

ShellRepl::ShellRepl()
    : parser(std::make_unique<AntlrParser>()),
      preprocessor(std::make_unique<Preprocessor>()) {
}

ExecutionResult ShellRepl::process_line(std::string_view raw_input) const {
    const std::string input =
        preprocessor->preprocess(raw_input, Environment::instance());

    const ParseResult parsed = parser->parse(input);
    if (!parsed.is_ok()) {
        ExecutionResult res;
        res.exit_code = 1;
        res.error_message = parsed.error_message;
        return res;
    }

    const PipelineNode &pipeline = *parsed.pipeline;

    // TODO
    // just dump AST
    for (const auto &cmd : pipeline.get_commands()) {
        std::cout << "cmd=" << cmd.get_name();
        std::cout << (cmd.get_kind() != CommandKind::kExternal ? "[builtin"
                                                               : "[external")
                  << ",kind=" << to_string(cmd.get_kind()) << "] args = {";
        for (std::size_t i = 0; i < cmd.get_args().size(); ++i) {
            std::cout << cmd.get_args().at(i);
            if (i != cmd.get_args().size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "}\n";
    }

    return ExecutionResult{};
}

int ShellRepl::run() const {
    std::string input;
    ExecutionResult execution_result{};
    // int last_exit_code = 0;

    while (!execution_result.should_exit) {
        print_prompt();
        if (!std::getline(std::cin, input)) {
            break;
        }

        if (is_blank(input)) {
            continue;
        }

        execution_result = process_line(input);
        if (!execution_result.error_message.empty()) {
            std::cout << execution_result.error_message << "\n";
            std::cout << std::flush;
        }
    }

    return execution_result.exit_code;
}

}  // namespace btft
