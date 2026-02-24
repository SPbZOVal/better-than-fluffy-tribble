#include "shell_repl.h"

// ANTLR
#include <antlr4-runtime.h>

// Other
#include <iostream>
#include <memory>
#include <string>
#include "environment.h"
#include "executor/executor.h"
#include "parser/antlr_parser.h"

namespace btft {

ShellRepl::ShellRepl()
    : parser(std::make_unique<parser::AntlrParser>()){
}

interpreter::ExecutionResult ShellRepl::ProcessLine(
    std::string_view input
) const {
    using interpreter::ExecutionResult;
    using interpreter::PipelineNode;

    auto &env = Environment::GetInstance();
    env.ClearLocal();

    const parser::ParseResult parsed = parser->Parse(input);
    if (!parsed.IsOk()) {
        env.ClearLocal();
        ExecutionResult res;
        res.exit_code = 1;
        res.error_message = parsed.error_message;
        return res;
    }

    ExecutionResult result{};
    if (const PipelineNode &pipeline = parsed.pipeline.value();
        !pipeline.Empty()) {
        result = interpreter::executor::ExecutePipeline(pipeline.GetCommands());
    }

    env.ClearLocal();
    return result;
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

    std::cout << '\n' << std::flush;
    return execution_result.exit_code;
}

}  // namespace btft
