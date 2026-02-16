#pragma once

#include "icommand.h"

namespace btft::interpreter::executor::commands {

/**
 * ExitCommand - terminates the interpreter
 *
 * This command terminates the interpreter with an optional exit code.
 * If no exit code is provided, it defaults to 0 (success).
 *
 * Examples:
 * - exit → exits with code 0
 * - exit 1 → exits with code 1
 *
 * Pipeline examples:
 * - echo "goodbye" | exit → outputs "goodbye\n" then exits
 * - cat file.txt | exit 2 → reads file then exits with code 2
 */
class ExitCommand final : public ICommand {
public:
    ExitCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;

    static std::shared_ptr<ICommand> createCommand() {
        return std::make_shared<ExitCommand>();
    }
};

}  // namespace btft::interpreter::executor::commands
