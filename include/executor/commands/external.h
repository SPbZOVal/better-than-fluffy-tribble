#pragma once

#include "icommand.h"

namespace btft::interpreter::executor::commands {

/**
 * ExternalCommand - executes external system programs
 *
 * This command executes external programs found in the system PATH.
 * It searches for the program and executes it with the provided arguments.
 *
 * Examples:
 * - ls -la → executes the ls command with -la arguments
 * - grep "pattern" file.txt → executes grep with pattern and file
 *
 * Pipeline examples:
 * - ls | grep ".txt" → lists files and filters for .txt files
 * - cat file.txt | sort → reads file and sorts the content
 */
class ExternalCommand final : public ICommand {
public:
    ExternalCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;

    static std::shared_ptr<ICommand> createCommand() {
        return std::make_shared<ExternalCommand>();
    }
};

}  // namespace btft::interpreter::executor::commands
