#include "echo.h"

ExecutionResult EchoCommand::Execute(
    const std::vector<std::string> &args,
    IChannel &inputChannel,
    IChannel &outputChannel
) {
    while (!inputChannel.isClosed()) {
        outputChannel.write(inputChannel.read());
    }

    return ExecutionResult{.returnCode = 0};
}