#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace btft {

enum class CommandKind : std::uint8_t {
    kCat,
    kEcho,
    kWc,
    kPwd,
    kExit,
    kExternal,
};

[[nodiscard]] std::string to_string(CommandKind kind);

class CommandNode final {
public:
    CommandNode(
        std::string name,
        CommandKind kind,
        std::vector<std::string> args
    )
        : name_(std::move(name)), kind_(kind), args_(std::move(args)) {
    }

    [[nodiscard]] std::string_view get_name() const noexcept {
        return name_;
    }

    [[nodiscard]] CommandKind get_kind() const noexcept {
        return kind_;
    }

    [[nodiscard]] const std::vector<std::string> &get_args() const noexcept {
        return args_;
    }

private:
    std::string name_;
    CommandKind kind_;
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

    [[nodiscard]] const std::vector<CommandNode> &get_commands() const noexcept {
        return commands;
    }

    void add_command(CommandNode command) {
        commands.push_back(std::move(command));
    }

private:
    std::vector<CommandNode> commands;
};

}  // namespace btft
