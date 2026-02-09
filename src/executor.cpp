#include "executor.h"
#include <iostream>
#include <thread>
#include "channel.h"
#include "commands/registry.h"

void SingleNodeExecution(
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel,
    const CommandNode &node
) {
    auto command = CommandsRegistry::GetInstance().getCommand(node.name);
    ExecutionResult result =
        command->Execute(node.args, inputChannel, outputChannel);
    outputChannel->closeChannel();

    if (result.returnCode != 0) {
        // TODO: send error status code to the channel
    }
}

ExecutionResult ExecutePipeline(const std::vector<CommandNode> &nodes) {
    std::vector<std::thread> pipeline;
    std::shared_ptr<Channel> commonChannel;

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        std::shared_ptr<IInputChannel> inputChannel;
        if (i == 0) {
            inputChannel = std::make_shared<InputStdChannel>();
        } else {
            inputChannel =
                dynamic_pointer_cast<IInputChannel, Channel>(commonChannel);
        }

        std::shared_ptr<IOutputChannel> outputChannel;
        if (i + 1 == nodes.size()) {
            outputChannel = std::make_shared<OutputStdChannel>();
        } else {
            outputChannel = std::make_shared<Channel>();
            commonChannel =
                dynamic_pointer_cast<Channel, IOutputChannel>(outputChannel);
        }

        pipeline.push_back(
            std::thread{
                SingleNodeExecution, inputChannel, outputChannel,
                std::cref(nodes[i])
            }
        );
    }

    for (auto &thread : pipeline) {
        thread.join();
    }

    ExecutionResult result;
    result.returnCode = 0;
    return result;
}