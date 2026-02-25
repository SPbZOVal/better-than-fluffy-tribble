#include "executor/commands/pwd.h"
#include <filesystem>
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult PwdCommand::Execute(
    const std::vector<std::string> & /*args*/,
    std::shared_ptr<IInputChannel> /*input_channel*/,
    std::shared_ptr<IOutputChannel> output_channel
) {
    const std::filesystem::path cwd = std::filesystem::current_path();
    output_channel->Write(cwd.string() + "\n");

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
