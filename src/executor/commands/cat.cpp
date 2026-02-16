#include "executor/commands/cat.h"
#include <fstream>
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult CatCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> input_channel,
    std::shared_ptr<IOutputChannel> output_channel
) {
    if (args.empty()) {
        while (!input_channel->IsClosed()) {
            output_channel->Write(input_channel->Read());
        }
        return ExecutionResult{};
    }

    for (const auto &filename : args) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return ExecutionResult{.exit_code = 1};
        }

        std::string line;
        while (std::getline(file, line)) {
            output_channel->Write(line + "\n");
        }
        file.close();
    }

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
