#include "executor/commands/external.h"
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "environment.h"

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
        // Prepare argv
        std::vector<char *> argv;
        for (const auto &arg : args) {
            argv.push_back(const_cast<char *>(arg.c_str()));
        }
        argv.push_back(nullptr);

        // Set environment variables
        auto &env = Environment::GetInstance();
        std::vector<std::string> env_strings = env.GetEnvironmentArray();
        for (const auto &env_str : env_strings) {
            // Parse KEY=VALUE
            size_t eq_pos = env_str.find('=');
            if (eq_pos != std::string::npos) {
                std::string key = env_str.substr(0, eq_pos);
                std::string value = env_str.substr(eq_pos + 1);
                setenv(key.c_str(), value.c_str(), 1);
            }
        }

        // Execute with PATH search
        execvp(args[0].c_str(), argv.data());

        // If execvp returns, it failed
        std::cerr << args[0] << ": command not found\n";
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
