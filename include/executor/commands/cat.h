#pragma once

#include "icommand.h"

namespace btft::interpreter::executor::commands {

/**
 * CatCommand - concatenates and outputs files or input data
 *
 * This command reads from files specified as arguments or from the input
 * channel if no files are provided, and writes the content to the output
 * channel.
 *
 * Examples:
 * - cat file.txt → outputs content of file.txt
 * - cat file1.txt file2.txt → concatenates both files
 * - echo "hello" | cat → outputs "hello\n"
 *
 * Pipeline examples:
 * - cat file.txt | wc → counts lines, words, bytes in file.txt
 * - echo "test" | cat | cat → outputs "test\n" (double cat)
 */
class CatCommand final : public ICommand {
public:
    CatCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> input_channel,
        std::shared_ptr<IOutputChannel> output_channel
    ) override;

    static std::shared_ptr<ICommand> CreateCommand() {
        return std::make_shared<CatCommand>();
    }
};

}  // namespace btft::interpreter::executor::commands
