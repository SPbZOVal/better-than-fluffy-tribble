#pragma once

#include <string>
#include <vector>

namespace NInterpetator {
struct CommandNode {
    std::string name;
    std::vector<std::string> args;
};

struct ExecutionResult {
    int returnCode;
};
}  // namespace NInterpetator