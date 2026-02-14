#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "common.h"
#include "executor/channel.h"

namespace interpretator::executor::commands {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual interpretator::ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) = 0;

    static std::shared_ptr<ICommand> createCommand() {
        throw std::runtime_error("Unimplemented");
    }
};

template <typename T>
concept DerivedFromICommand =
    std::derived_from<T, commands::ICommand> && requires(T) {
        {
            T::createCommand()
        } -> std::same_as<std::shared_ptr<commands::ICommand>>;
    };

}  // namespace interpretator::executor::commands
