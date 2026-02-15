#include "executor/commands/external.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

namespace interpretator::executor::commands {

interpretator::ExecutionResult ExternalCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    if (args.empty()) {
        return interpretator::ExecutionResult{.returnCode = 1, .shouldExit = false};
    }

    pid_t pid = fork();
    if (pid == -1) {
        return interpretator::ExecutionResult{.returnCode = 1, .shouldExit = false};
    }

    if (pid == 0) {
        std::vector<char*> argv;
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);
        execvp(args[0].c_str(), argv.data());
        std::exit(127);
    } else {
        int status;
        waitpid(pid, &status, 0);

        interpretator::ExecutionResult result;
        result.shouldExit = false;

        if (WIFEXITED(status)) {
            result.returnCode = WEXITSTATUS(status);
        } else {
            result.returnCode = 1;
        }

        return result;
    }
}

}  // namespace interpretator::executor::commands
