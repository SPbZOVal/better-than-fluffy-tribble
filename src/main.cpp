#include "executor/commands/cat.h"
#include "executor/commands/echo.h"
#include "executor/commands/exit.h"
#include "executor/commands/grep.h"
#include "executor/commands/pwd.h"
#include "executor/commands/registry.h"
#include "executor/commands/wc.h"
#include "shell_repl.h"

int main() {
    // NOLINTNEXTLINE
    using namespace btft::interpreter::executor;

    // Register all built-in commands
    auto &registry = CommandsRegistry::GetInstance();
    registry.RegisterCommand<commands::EchoCommand>("echo");
    registry.RegisterCommand<commands::CatCommand>("cat");
    registry.RegisterCommand<commands::PwdCommand>("pwd");
    registry.RegisterCommand<commands::WcCommand>("wc");
    registry.RegisterCommand<commands::ExitCommand>("exit");
    registry.RegisterCommand<commands::GrepCommand>("grep");

    const btft::ShellRepl repl;
    return repl.Run();
}
