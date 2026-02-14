#pragma once

#include "common.h"

namespace interpretator::executor {

interpretator::ExecutionResult ExecutePipeline(
    const std::vector<interpretator::CommandNode> &nodes
);

}  // namespace interpretator::executor
