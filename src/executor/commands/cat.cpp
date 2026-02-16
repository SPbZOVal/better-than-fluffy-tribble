#include "executor/commands/cat.h"
#include <fstream>
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult CatCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    if (args.empty()) {
        while (!inputChannel->isClosed()) {
            outputChannel->write(inputChannel->read());
        }
        return ExecutionResult{};
    }

    for (const auto& filename : args) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return ExecutionResult{.exit_code = 1};
        }

        std::string line;
        while (std::getline(file, line)) {
            outputChannel->write(line + "\n");
        }
        file.close();
    }

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
