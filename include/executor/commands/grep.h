#pragma once

#include <memory>
#include <string>
#include <vector>
#include "common.h"
#include "executor/channel.h"
#include "icommand.h"

namespace btft::interpreter::executor::commands {

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
