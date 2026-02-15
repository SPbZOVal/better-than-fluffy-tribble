#include "executor/executor.h"
#include <iostream>
#include <thread>
#include "executor/channel.h"
#include "executor/commands/registry.h"

namespace btft::interpreter::executor {

static void SingleNodeExecution(
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel,
    const CommandNode &node
) {
    auto command = CommandsRegistry::GetInstance().getCommand(node.get_name());
    ExecutionResult result =
        command->Execute(node.get_args(), inputChannel, outputChannel);
    outputChannel->closeChannel();

    if (result.exit_code != 0) {
        // TODO: send error status code to the channel
    }
}

ExecutionResult ExecutePipeline(const std::vector<CommandNode> &nodes) {
    std::vector<std::thread> pipeline;

    std::vector<std::shared_ptr<IInputChannel>> inputChannels(
        nodes.size(), nullptr
    );
    std::vector<std::shared_ptr<IOutputChannel>> outputChannels(
        nodes.size(), nullptr
    );

    for (std::size_t i = 0; i + 1 < nodes.size(); ++i) {
        std::shared_ptr<Channel> commonChannel = std::make_shared<Channel>();
        // splits assignment into 2 lines
        // clang-format off
        inputChannels[i + 1] = dynamic_pointer_cast<IInputChannel, Channel>(commonChannel);
        outputChannels[i] = dynamic_pointer_cast<IOutputChannel, Channel>(commonChannel);
        // clang-format on
    }

    // create channels for std::cout and std::cin
    inputChannels.front() = std::make_shared<InputStdChannel>();
    outputChannels.back() = std::make_shared<OutputStdChannel>();

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        pipeline.push_back(
            std::thread{
                SingleNodeExecution, inputChannels[i], outputChannels[i],
                std::cref(nodes[i])
            }
        );
    }

    for (auto &thread : pipeline) {
        thread.join();
    }

    ExecutionResult result;
    result.exit_code = 0;
    return result;
}

}  // namespace btft::interpreter::executor
