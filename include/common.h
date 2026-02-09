#pragma once

#include <string>
#include <vector>

struct CommandNode {
    std::string name;
    std::vector<std::string> args;
};

struct ExecutionResult {
    int returnCode;
};
