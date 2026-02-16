#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace btft::interpreter {

class CommandNode final {
public:
    CommandNode(std::string name, std::vector<std::string> args)
        : name(std::move(name)), args(std::move(args)) {
    }

    [[nodiscard]] const std::string &GetName() const noexcept {
        return name;
    }

    [[nodiscard]] const std::vector<std::string> &GetArgs() const noexcept {
        return args;
    }

private:
    std::string name;
    std::vector<std::string> args;
};

class PipelineNode final {
public:
    PipelineNode() = default;

    explicit PipelineNode(std::vector<CommandNode> commands)
        : commands(std::move(commands)) {
    }

    [[nodiscard]] bool Empty() const noexcept {
        return commands.empty();
    }

    [[nodiscard]] std::size_t Size() const noexcept {
        return commands.size();
    }

    [[nodiscard]] const std::vector<CommandNode> &GetCommands(
    ) const noexcept {
        return commands;
    }

    void AddCommand(CommandNode command) {
        commands.push_back(std::move(command));
    }

private:
    std::vector<CommandNode> commands;
};

struct ExecutionResult {
    int exit_code = 0;
    bool should_exit = false;
    std::string error_message;
};

}  // namespace btft::interpreter
