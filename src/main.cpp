#include <iostream>
#include "commands/echo.h"
#include "commands/registry.h"
#include "common.h"
#include "executor.h"

int main() {
    NInterpretator::NExecutor::CommandsRegistry::GetInstance().registerCommand(
        "echo",
        []() {
            return std::make_shared<
                NInterpretator::NExecutor::NCommands::EchoCommand>();
        }
    );
    NInterpetator::CommandNode node{.name = "echo", .args = {}};

    std::vector<NInterpetator::CommandNode> a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(node);
    }
    NInterpretator::NExecutor::ExecutePipeline(a);
    return 0;
}