#include "executor/commands/cat.h"
#include "executor/commands/echo.h"
#include "executor/commands/exit.h"
#include "executor/commands/pwd.h"
#include "executor/commands/registry.h"
#include "executor/commands/wc.h"
#include "shell_repl.h"

int main() {
    // Register all built-in commands
    auto &registry =
        btft::interpreter::executor::CommandsRegistry::GetInstance();
    registry
        .registerCommand<btft::interpreter::executor::commands::EchoCommand>(
            "echo"
        );
    registry.registerCommand<btft::interpreter::executor::commands::CatCommand>(
        "cat"
    );
    registry.registerCommand<btft::interpreter::executor::commands::PwdCommand>(
        "pwd"
    );
    registry.registerCommand<btft::interpreter::executor::commands::WcCommand>(
        "wc"
    );
    registry
        .registerCommand<btft::interpreter::executor::commands::ExitCommand>(
            "exit"
        );

    const btft::ShellRepl repl;
    return repl.run();
}
