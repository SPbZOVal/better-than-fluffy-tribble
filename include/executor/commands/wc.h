#pragma once

#include "icommand.h"

namespace btft::interpreter::executor::commands {

/**
 * WcCommand - counts lines, words, and bytes in files or input data
 *
 * This command counts the number of lines, words, and bytes in files specified
 * as arguments or from the input channel if no files are provided.
 *
 * Examples:
 * - wc file.txt → outputs "lines words bytes file.txt"
 * - echo "hello world" | wc → outputs "1 2 12"
 *
 * Pipeline examples:
 * - cat file.txt | wc → counts lines, words, bytes in file.txt
 * - echo "test" | wc | cat → outputs "1 1 5\n"
 */
class WcCommand final : public ICommand {
public:
    WcCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;

    static std::shared_ptr<ICommand> CreateCommand() {
        return std::make_shared<WcCommand>();
    }
};

}  // namespace btft::interpreter::executor::commands
