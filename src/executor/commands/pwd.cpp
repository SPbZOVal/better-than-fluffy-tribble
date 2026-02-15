#include "executor/commands/pwd.h"
#include <filesystem>
#include <iostream>

namespace interpretator::executor::commands {

interpretator::ExecutionResult PwdCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    std::filesystem::path cwd = std::filesystem::current_path();
    outputChannel->write(cwd.string() + "\n");

    return interpretator::ExecutionResult{.returnCode = 0};
}

}  // namespace interpretator::executor::commands
