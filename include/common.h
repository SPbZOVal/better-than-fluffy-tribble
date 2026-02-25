#pragma once

#include <string>
#include <utility>
#include <vector>

namespace btft::interpreter {

struct ArgSegment {
    std::string text;
    bool allow_expansion = true;
};

struct ArgToken {
    std::vector<ArgSegment> segments;

    [[nodiscard]] bool Empty() const noexcept {
        return segments.empty();
    }
};

class CommandNode final {
public:
    CommandNode(ArgToken name, std::vector<ArgToken> args)
        : name(std::move(name)), args(std::move(args)) {
    }

    [[nodiscard]] const ArgToken &GetName() const noexcept {
        return name;
    }

    [[nodiscard]] const std::vector<ArgToken> &GetArgs() const noexcept {
        return args;
    }

private:
    ArgToken name;
    std::vector<ArgToken> args;
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

    [[nodiscard]] const std::vector<CommandNode> &GetCommands() const noexcept {
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

inline bool IsVarStart(char c) noexcept {
    return (std::isalpha(static_cast<unsigned char>(c)) != 0) || c == '_';
}

inline bool IsVarChar(char c) noexcept {
    return (std::isalnum(static_cast<unsigned char>(c)) != 0) || c == '_';
}

}  // namespace btft::interpreter