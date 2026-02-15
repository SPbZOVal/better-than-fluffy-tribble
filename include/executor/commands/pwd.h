#pragma once

#include "icommand.h"

namespace interpretator::executor::commands {

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
    interpretator::ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;

    static std::shared_ptr<ICommand> createCommand() {
        return std::make_shared<PwdCommand>();
    }
};

}  // namespace interpretator::executor::commands
