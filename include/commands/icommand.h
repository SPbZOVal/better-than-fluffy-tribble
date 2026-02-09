#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "channel.h"
#include "common.h"

namespace NInterpretator::NExecutor::NCommands {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual NInterpetator::ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) = 0;
};

using CommandCreator = std::function<std::shared_ptr<ICommand>()>;
}  // namespace NInterpretator::NExecutor::NCommands