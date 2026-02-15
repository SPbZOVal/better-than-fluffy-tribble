#pragma once

#include <memory>
#include <string>
#include <vector>
#include "common.h"
#include "executor/channel.h"
#include "icommand.h"

namespace btft::interpreter::executor::commands {

/**
 * EchoCommand - outputs its arguments to the output channel
 *
 * This command writes all provided arguments to the output channel,
 * separated by spaces and followed by a newline. It ignores input channel.
 *
 * Examples:
 * - echo hello world → outputs "hello world\n"
 * - echo "test message" → outputs "test message\n"
 * - echo → outputs just a newline
 *
 * Pipeline examples:
 * - echo "hello" | cat → outputs "hello\n" through cat
 * - echo "test" | wc → counts lines, words, and bytes of "test\n"
 */
class EchoCommand final : public ICommand {
public:
    EchoCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;

    static std::shared_ptr<ICommand> createCommand() {
        return std::make_shared<EchoCommand>();
    }
};

}  // namespace btft::interpreter::executor::commands
