#pragma once

#include "icommand.h"

namespace btft::interpreter::executor::commands {

/**
 * PwdCommand - prints the current working directory
 *
 * This command outputs the absolute path of the current working directory
 * to the output channel.
 *
 * Examples:
 * - pwd → outputs "/home/user/project\n"
 *
 * Pipeline examples:
 * - pwd | cat → outputs current directory path
 * - echo $(pwd) → would output the directory path
 */
class PwdCommand final : public ICommand {
public:
    PwdCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> input_channel,
        std::shared_ptr<IOutputChannel> output_channel
    ) override;

    static std::shared_ptr<ICommand> CreateCommand() {
        return std::make_shared<PwdCommand>();
    }
};

}  // namespace btft::interpreter::executor::commands
