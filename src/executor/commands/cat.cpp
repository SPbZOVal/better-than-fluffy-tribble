#include "executor/commands/cat.h"
#include <fstream>
#include <iostream>

namespace interpretator::executor::commands {

interpretator::ExecutionResult CatCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    if (args.empty()) {
        while (!inputChannel->isClosed()) {
            outputChannel->write(inputChannel->read());
        }
        return interpretator::ExecutionResult{.returnCode = 0};
    }

    for (const auto& filename : args) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return interpretator::ExecutionResult{.returnCode = 1};
        }

        std::string line;
        while (std::getline(file, line)) {
            outputChannel->write(line + "\n");
        }
        file.close();
    }

    return interpretator::ExecutionResult{.returnCode = 0};
}

}  // namespace interpretator::executor::commands
