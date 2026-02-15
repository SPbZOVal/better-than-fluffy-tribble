#pragma once

#include <string>
#include <vector>

namespace btft::interpreter {

class CommandNode final {
public:
    CommandNode(std::string name, std::vector<std::string> args)
        : name_(std::move(name)), args_(std::move(args)) {
    }

    [[nodiscard]] const std::string &get_name() const noexcept {
        return name_;
    }

    [[nodiscard]] const std::vector<std::string> &get_args() const noexcept {
        return args_;
    }

private:
    std::string name_;
    std::vector<std::string> args_;
};

class PipelineNode final {
public:
    PipelineNode() = default;

    explicit PipelineNode(std::vector<CommandNode> commands)
        : commands(std::move(commands)) {
    }

    [[nodiscard]] bool empty() const noexcept {
        return commands.empty();
    }

    [[nodiscard]] size_t size() const noexcept {
        return commands.size();
    }

    [[nodiscard]] const std::vector<CommandNode> &get_commands(
    ) const noexcept {
        return commands;
    }

    void add_command(CommandNode command) {
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
