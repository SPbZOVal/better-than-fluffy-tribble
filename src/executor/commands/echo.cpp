#include "executor/commands/echo.h"
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult EchoCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    while (!inputChannel->isClosed()) {
        outputChannel->write(inputChannel->read());
    }

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
