#include "shell_repl.h"

// ANTLR
#include <antlr4-runtime.h>

// Other
#include <iostream>
#include <memory>
#include <string>
#include "../include/executor/executor.h"
#include "../include/parser/antlr_parser.h"
#include "../include/parser/preprocessor.h"
#include "environment.h"

namespace btft {

ShellRepl::ShellRepl()
    : parser(std::make_unique<parser::AntlrParser>()),
      preprocessor(std::make_unique<parser::Preprocessor>()) {
}

interpreter::ExecutionResult ShellRepl::ProcessLine(std::string_view raw_input
) const {
    using interpreter::ExecutionResult;
    using interpreter::PipelineNode;

    const std::string input = preprocessor->Preprocess(raw_input);

    const parser::ParseResult parsed = parser->Parse(input);
    if (!parsed.IsOk()) {
        ExecutionResult res;
        res.exit_code = 1;
        res.error_message = parsed.error_message;
        return res;
    }

    const PipelineNode &pipeline = parsed.pipeline.value();

    if (!pipeline.Empty()) {
        return interpreter::executor::ExecutePipeline(pipeline.GetCommands());
    }

    return ExecutionResult{};
}

int ShellRepl::Run() const {
    using interpreter::ExecutionResult;

    std::string input;
    ExecutionResult execution_result{};

    while (!execution_result.should_exit) {
        PrintPrompt();
        if (!std::getline(std::cin, input)) {
            break;
        }

        if (IsBlank(input)) {
            continue;
        }

        execution_result = ProcessLine(input);
        if (!execution_result.error_message.empty()) {
            std::cout << execution_result.error_message << "\n";
            std::cout << std::flush;
        }
    }
    std::cout << std::endl;

    return execution_result.exit_code;
}

}  // namespace btft
