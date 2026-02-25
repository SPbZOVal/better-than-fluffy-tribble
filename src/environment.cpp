#include <environment.h>
#include <utility>

namespace btft {

void Environment::SetLocal(const std::string &name, const std::string &value) {
    local_vars[name] = value;
}

std::optional<std::string> Environment::GetLocal(const std::string &name
) const {
    if (const auto it = local_vars.find(name); it == local_vars.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

bool Environment::HasLocal(const std::string &name) const {
    return local_vars.contains(name);
}

void Environment::SetGlobal(const std::string &name, const std::string &value) {
    global_vars[name] = value;
}

std::optional<std::string> Environment::GetGlobal(const std::string &name
) const {
    if (const auto it = global_vars.find(name); it == global_vars.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

bool Environment::HasGlobal(const std::string &name) const {
    return global_vars.contains(name);
}

bool Environment::HasVar(const std::string &name) const {
    return HasLocal(name) || HasGlobal(name);
}

std::optional<std::string> Environment::GetVar(const std::string &name) const {
    if (const auto local_var = GetLocal(name); local_var) {
        return local_var;
    }
    return GetGlobal(name);
}

void Environment::ClearLocal() {
    local_vars.clear();
}

std::vector<std::string> Environment::GetEnvironmentArray() const {
    std::vector<std::string> result;

    // Add global variables
    for (const auto &[key, value] : global_vars) {
        result.push_back(key + "=" + value);
    }

    // Add local variables (they override globals if same key exists)
    for (const auto &[key, value] : local_vars) {
        result.push_back(key + "=" + value);
    }

    return result;
}
}  // namespace btft
