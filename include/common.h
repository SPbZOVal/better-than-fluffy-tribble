#pragma once

#include <string>
#include <vector>

namespace interpretator {
struct CommandNode {
    std::string name;
    std::vector<std::string> args;
};

struct ExecutionResult {
    int returnCode;
};
}  // namespace interpretator
