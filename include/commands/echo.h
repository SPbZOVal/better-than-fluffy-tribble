#pragma once

#include "commands/icommand.h"

class EchoCommand final : public ICommand {
public:
    ExecutionResult Execute(
        const std::vector<std::string> &args,
        IChannel &inputChannel,
        IChannel &outputChannel
    ) override;
};