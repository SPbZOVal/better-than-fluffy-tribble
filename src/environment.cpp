#include "environment.h"
#include <utility>

namespace btft {

void Environment::set(std::string name, std::string value) {
    vars[std::move(name)] = std::move(value);
}

std::optional<std::string> Environment::get(const std::string &name) const {
    if (const auto it = vars.find(name); it == vars.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

bool Environment::has(const std::string &name) const {
    return vars.contains(name);
}

}  // namespace btft
