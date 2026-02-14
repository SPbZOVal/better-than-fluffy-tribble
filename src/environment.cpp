#include "environment.h"
#include <utility>

namespace btft {

void Environment::set_local(const std::string &name, const std::string &value) {
    local_vars[name] = value;
}

std::optional<std::string> Environment::get_local(
    const std::string &name
) const {
    if (const auto it = local_vars.find(name); it == local_vars.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

bool Environment::has_local(const std::string &name) const {
    return local_vars.contains(name);
}

void Environment::set_global(
    const std::string &name,
    const std::string &value
) {
    global_vars[name] = value;
}

std::optional<std::string> Environment::get_global(
    const std::string &name
) const {
    if (const auto it = global_vars.find(name); it == global_vars.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

bool Environment::has_global(const std::string &name) const {
    return global_vars.contains(name);
}

}  // namespace btft
