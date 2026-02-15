#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <vector>
#include "common.h"
#include "executor/channel.h"

namespace btft::interpreter::executor::commands {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual ExecutionResult Execute(
        const std::vector<std::string> &args,
        std::shared_ptr<IInputChannel> inputChannel,
        std::shared_ptr<IOutputChannel> outputChannel
    ) = 0;

    static std::shared_ptr<ICommand> createCommand() {
        throw std::runtime_error("Unimplemented");
    }
};

template <typename T>
concept DerivedFromICommand = std::derived_from<T, ICommand> && requires(T) {
    { T::createCommand() } -> std::same_as<std::shared_ptr<ICommand>>;
};

}  // namespace btft::interpreter::executor::commands
