#include "executor/executor.h"
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include "executor/channel.h"
#include "executor/commands/registry.h"

namespace btft::interpreter::executor {

struct PipelineState {
    std::atomic<bool> should_stop{false};
    std::atomic<int> exit_code{0};
    std::atomic<bool> should_exit{false};
    std::mutex mutex;
};

static void SingleNodeExecution(
    std::shared_ptr<IInputChannel> input_channel,
    std::shared_ptr<IOutputChannel> output_channel,
    const CommandNode &node,
    std::shared_ptr<PipelineState> state
) {
    // Check if pipeline should stop before executing
    if (state->should_stop.load()) {
        output_channel->CloseChannel();
        return;
    }

    const auto command =
        CommandsRegistry::GetInstance().GetCommand(node.GetName());
    const ExecutionResult result =
        command->Execute(node.GetArgs(), input_channel, output_channel);
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
        pipeline.push_back(
            std::thread{
                SingleNodeExecution, input_channels[i], output_channels[i],
                std::cref(nodes[i]), state
            }
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
