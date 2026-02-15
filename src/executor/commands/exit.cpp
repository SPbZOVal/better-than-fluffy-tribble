#include "executor/commands/exit.h"
#include <iostream>
#include <cstdlib>

namespace interpretator::executor::commands {

interpretator::ExecutionResult ExitCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    int exit_code = 0;

    if (!args.empty()) {
        try {
            exit_code = std::stoi(args[0]);
        } catch (const std::exception& e) {
            exit_code = 0;
        }
    }

    interpretator::ExecutionResult result;
    result.returnCode = exit_code;
    result.shouldExit = true;

    return result;
}

}  // namespace interpretator::executor::commands
