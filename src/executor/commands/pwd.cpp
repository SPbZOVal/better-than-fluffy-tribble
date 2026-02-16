#include "executor/commands/pwd.h"
#include <filesystem>
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult PwdCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    std::filesystem::path cwd = std::filesystem::current_path();
    outputChannel->write(cwd.string() + "\n");

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
