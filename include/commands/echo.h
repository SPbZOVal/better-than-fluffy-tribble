#pragma once

#include "commands/icommand.h"

class EchoCommand final : public ICommand {
public:
    EchoCommand() = default;
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) override;
};