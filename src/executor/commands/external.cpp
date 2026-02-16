#include "executor/commands/external.h"
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

namespace btft::interpreter::executor::commands {

ExecutionResult ExternalCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> input_channel,
    std::shared_ptr<IOutputChannel> output_channel
) {
    if (args.empty()) {
        return ExecutionResult{.exit_code = 1, .should_exit = false};
    }

    pid_t pid = fork();
    if (pid == -1) {
        return ExecutionResult{.exit_code = 1, .should_exit = false};
    }

    if (pid == 0) {
        std::vector<char *> argv;
        for (const auto &arg : args) {
            argv.push_back(const_cast<char *>(arg.c_str()));
        }
        argv.push_back(nullptr);
        execvp(args[0].c_str(), argv.data());
        std::exit(127);
    } else {
        int status;
        waitpid(pid, &status, 0);

        ExecutionResult result;
        result.should_exit = false;

        if (WIFEXITED(status)) {
            result.exit_code = WEXITSTATUS(status);
        } else {
            result.exit_code = 1;
        }

        return result;
    }
}

}  // namespace btft::interpreter::executor::commands
