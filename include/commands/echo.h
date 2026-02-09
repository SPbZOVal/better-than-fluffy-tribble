#pragma once

#include "commands/icommand.h"

namespace NInterpretator::NExecutor::NCommands {

class EchoCommand final : public ICommand {
public:
    EchoCommand() = default;
    NInterpetator::ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;
};

}  // namespace NInterpretator::NExecutor::NCommands