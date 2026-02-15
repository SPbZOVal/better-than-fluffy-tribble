#include "shell_repl.h"

// ANTLR
#include <antlr4-runtime.h>

// Other
#include <iostream>
#include <memory>
#include <string>
#include "../include/parser/antlr_parser.h"
#include "../include/parser/preprocessor.h"
#include "environment.h"

namespace btft {

ShellRepl::ShellRepl()
    : parser(std::make_unique<parser::AntlrParser>()),
      preprocessor(std::make_unique<parser::Preprocessor>()) {
}

interpreter::ExecutionResult ShellRepl::process_line(
    std::string_view raw_input
) const {
    using interpreter::ExecutionResult;
    using interpreter::PipelineNode;

    const std::string input = preprocessor->preprocess(raw_input);

    const parser::ParseResult parsed = parser->parse(input);
    if (!parsed.is_ok()) {
        ExecutionResult res;
        res.exit_code = 1;
        res.error_message = parsed.error_message;
        return res;
    }

    [[maybe_unused]] const PipelineNode &pipeline = parsed.pipeline.value();

    // TODO
    return ExecutionResult{};
}

int ShellRepl::run() const {
    using interpreter::ExecutionResult;

    std::string input;
    ExecutionResult execution_result{};

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
