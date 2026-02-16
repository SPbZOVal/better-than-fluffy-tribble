#include "executor/commands/exit.h"
#include <cstdlib>
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult ExitCommand::
    Execute(const std::vector<std::string> &args, std::shared_ptr<IInputChannel>, std::shared_ptr<IOutputChannel>) {
    int exit_code = 0;

    if (!args.empty()) {
        try {
            exit_code = std::stoi(args[0]);
        } catch (const std::exception &e) {
            exit_code = 0;
        }
    }

    ExecutionResult result;
    result.exit_code = exit_code;
    result.should_exit = true;
    return result;
}

}  // namespace btft::interpreter::executor::commands
