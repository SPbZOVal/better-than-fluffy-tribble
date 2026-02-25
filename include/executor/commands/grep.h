#pragma once

#include <memory>
#include <string>
#include <vector>
#include "common.h"
#include "executor/channel.h"
#include "icommand.h"

namespace btft::interpreter::executor::commands {

/**
 * @brief Command implementation for the 'grep' utility - searches for patterns
 * in text
 *
 * This class implements the ICommand interface to provide grep-like
 * functionality for searching text patterns. It inherits from ICommand and is
 * designed to be used in a command execution pipeline.
 *
 * The class follows the Factory Method pattern with CreateCommand() to allow
 * dynamic creation of command instances.
 */
class GrepCommand final : public ICommand {
public:
    GrepCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> input_channel,
        std::shared_ptr<IOutputChannel> output_channel
    ) override;

    static std::shared_ptr<ICommand> CreateCommand() {
        return std::make_shared<GrepCommand>();
    }
};

}  // namespace btft::interpreter::executor::commands
