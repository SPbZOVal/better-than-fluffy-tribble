#include "executor/commands/echo.h"
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult EchoCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    for (size_t i = 0; i < args.size(); ++i) {
        outputChannel->write(args[i]);
        if (i != args.size() - 1) {
            outputChannel->write(" ");
        }
    }
    outputChannel->write("\n");

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
