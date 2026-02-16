#pragma once

#include "common.h"

namespace btft::interpreter::executor {

ExecutionResult ExecutePipeline(const std::vector<CommandNode> &nodes);

}  // namespace btft::interpreter::executor
