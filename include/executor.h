#pragma once

#include "common.h"

namespace NInterpretator::NExecutor {

NInterpetator::ExecutionResult ExecutePipeline(
    const std::vector<NInterpetator::CommandNode> &nodes
);

}  // namespace NInterpretator::NExecutor