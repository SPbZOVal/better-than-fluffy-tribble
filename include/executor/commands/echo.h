#pragma once

#include "icommand.h"

namespace btft::interpreter::executor::commands {

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
