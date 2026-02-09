#include "commands/echo.h"
#include <iostream>

namespace NInterpretator::NExecutor::NCommands {

NInterpetator::ExecutionResult EchoCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    while (!inputChannel->isClosed()) {
        outputChannel->write(inputChannel->read());
    }

    return NInterpetator::ExecutionResult{.returnCode = 0};
}

}  // namespace NInterpretator::NExecutor::NCommands