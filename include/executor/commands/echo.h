#pragma once

#include "icommand.h"

namespace interpretator::executor::commands {

class EchoCommand final : public ICommand {
public:
    EchoCommand() = default;
    interpretator::ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;

    static std::shared_ptr<ICommand> createCommand() {
        return std::make_shared<EchoCommand>();
    }
};

}  // namespace interpretator::executor::commands
