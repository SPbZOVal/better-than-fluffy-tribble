#include "executor/commands/echo.h"
#include <iostream>

namespace interpretator::executor::commands {

interpretator::ExecutionResult EchoCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    while (!inputChannel->isClosed()) {
        outputChannel->write(inputChannel->read());
    }

    return interpretator::ExecutionResult{.returnCode = 0};
}

}  // namespace interpretator::executor::commands
