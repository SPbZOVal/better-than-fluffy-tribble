#include "executor/executor.h"
#include <environment.h>
#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include "executor/channel.h"
#include "executor/commands/registry.h"

namespace btft::interpreter::executor {

namespace {

struct PipelineState {
    std::atomic<bool> should_stop{false};
    std::atomic<int> exit_code{0};
    std::atomic<bool> should_exit{false};
};

std::string ExpandVars(std::string_view s) {
    std::string out;
    out.reserve(s.size());

    for (std::size_t i = 0; i < s.size(); ++i) {
        if (const char c = s[i]; c != '$') {
            out.push_back(c);
            continue;
        }

        if (i + 1 >= s.size() || !IsVarStart(s[i + 1])) {
            out.push_back('$');
            continue;
        }

        std::size_t j = i + 1;
        while (j < s.size() && IsVarChar(s[j])) {
            ++j;
        }

        const std::string name(s.substr(i + 1, j - (i + 1)));
        if (const auto val = Environment::GetInstance().GetVar(name);
            val.has_value()) {
            out += *val;
        }

        i = j - 1;
    }

    return out;
}

[[nodiscard]] std::string ExpandArgToken(const interpreter::ArgToken &tok) {
    std::string out;

    for (const auto &seg : tok.segments) {
        if (!seg.allow_expansion) {
            out += seg.text;
        } else {
            out += ExpandVars(seg.text);
        }
    }

    return out;
}

struct ExpandedCommand {
    std::string name;
    std::vector<std::string> args;
};

ExpandedCommand ExpandCommandNode(const CommandNode &node) {
    ExpandedCommand out;

    out.name = ExpandArgToken(node.GetName());
    for (const auto &a : node.GetArgs()) {
        out.args.push_back(ExpandArgToken(a));
    }

    return out;
}

void SingleNodeExecution(
    const std::shared_ptr<IInputChannel> &input_channel,
    const std::shared_ptr<IOutputChannel> &output_channel,
    const CommandNode &node,
    const std::shared_ptr<PipelineState> &state
) {
    // Check if pipeline should stop before executing
    if (state->should_stop.load()) {
        output_channel->CloseChannel();
        return;
    }

    const ExpandedCommand expanded = ExpandCommandNode(node);

    ExecutionResult result{};
    const auto command =
        CommandsRegistry::GetInstance().GetCommand(expanded.name);
    if (dynamic_cast<commands::ExternalCommand *>(command.get()) == nullptr) {
        result = command->Execute(expanded.args, input_channel, output_channel);
    } else {
        std::vector<std::string> argv;
        argv.reserve(expanded.args.size() + 1);
        argv.push_back(expanded.name);
        argv.insert(argv.end(), expanded.args.begin(), expanded.args.end());
        result = command->Execute(argv, input_channel, output_channel);
    }

    output_channel->CloseChannel();

    // Update pipeline state if command failed or requested exit
    if (result.exit_code != 0 || result.should_exit) {
        bool expected = false;
        if (state->should_stop.compare_exchange_strong(expected, true)) {
            // First thread to set error state
            state->exit_code.store(result.exit_code);
            state->should_exit.store(result.should_exit);
        }
    }
}

}  // namespace

ExecutionResult ExecutePipeline(const std::vector<CommandNode> &nodes) {
    std::vector<std::thread> pipeline;
    auto state = std::make_shared<PipelineState>();

    std::vector<std::shared_ptr<IInputChannel>> input_channels(
        nodes.size(), nullptr
    );
    std::vector<std::shared_ptr<IOutputChannel>> output_channels(
        nodes.size(), nullptr
    );

    for (std::size_t i = 0; i + 1 < nodes.size(); ++i) {
        auto common_channel = std::make_shared<Channel>();
        // splits assignment into 2 lines
        // clang-format off
        input_channels[i + 1] = dynamic_pointer_cast<IInputChannel, Channel>(common_channel);
        output_channels[i] = dynamic_pointer_cast<IOutputChannel, Channel>(common_channel);
        // clang-format on
    }

    // create channels for std::cout and std::cin
    input_channels.front() = std::make_shared<InputStdChannel>();
    output_channels.back() = std::make_shared<OutputStdChannel>();

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        pipeline.emplace_back(
            SingleNodeExecution, input_channels[i], output_channels[i],
            std::cref(nodes[i]), state

        );
    }

    for (auto &thread : pipeline) {
        thread.join();
    }

    ExecutionResult result;
    result.exit_code = state->exit_code.load();
    result.should_exit = state->should_exit.load();
    return result;
}

}  // namespace btft::interpreter::executor
