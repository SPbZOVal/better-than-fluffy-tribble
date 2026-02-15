#pragma once

#include <optional>
#include <string>
#include "common.h"

namespace btft::parser {

struct ParseResult {
    std::optional<interpreter::PipelineNode> pipeline;  // NOLINT
    std::string error_message;                          // NOLINT

    static ParseResult ok(interpreter::PipelineNode p) {
        ParseResult r;
        r.pipeline = std::move(p);
        return r;
    }

    static ParseResult error(std::string msg) {
        ParseResult r;
        r.error_message = std::move(msg);
        return r;
    }

    [[nodiscard]] bool is_ok() const noexcept {
        return pipeline.has_value();
    }
};

class IParser {
public:
    IParser() = default;
    [[nodiscard]] virtual ParseResult parse(std::string_view input) const = 0;
    virtual ~IParser() = default;

    IParser(const IParser &) = delete;
    IParser(IParser &&) = delete;
    IParser &operator=(const IParser &) = delete;
    IParser &operator=(IParser &&) = delete;
};

}  // namespace btft::parser