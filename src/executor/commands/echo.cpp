#include "executor/commands/echo.h"
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult EchoCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> input_channel,
    std::shared_ptr<IOutputChannel> output_channel
) {
    for (std::size_t i = 0; i < args.size(); ++i) {
        output_channel->Write(args[i]);
        if (i != args.size() - 1) {
            output_channel->Write(" ");
        }
    }
    output_channel->Write("\n");

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
