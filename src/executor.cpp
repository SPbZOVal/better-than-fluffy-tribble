#include "executor.h"
#include <thread>
#include "channel.h"
#include "commands/registry.h"

void SingleNodeExecution(
    IChannel &inputChannel,
    IChannel &outputChannel,
    const CommandNode &node
) {
    auto command = CommandsRegistry::GetInstance().getCommand(node.name);
    ExecutionResult result =
        command->Execute(node.args, inputChannel, outputChannel);
    outputChannel.closeChannel();

    if (result.returnCode != 0) {
        // TODO: send error status code to the channel
    }
}

ExecutionResult ExecutePipeline(const std::vector<CommandNode> &nodes) {
    std::vector<std::thread> pipeline;
    std::shared_ptr<IChannel> inputChannel;
    std::shared_ptr<IChannel> outputChannel = std::make_shared<InputChannel>();

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        inputChannel = std::move(outputChannel);
        if (i == nodes.size() - 1) {
            outputChannel = std::make_shared<OutputChannel>();
        } else {
            outputChannel = std::make_shared<Channel>();
        }

        pipeline.emplace_back(
            SingleNodeExecution, std::ref(*inputChannel),
            std::ref(*outputChannel), std::cref(nodes[i])
        );
    }

    for (auto &thread : pipeline) {
        thread.join();
    }

    ExecutionResult result;
    result.returnCode = 0;
    return result;
}