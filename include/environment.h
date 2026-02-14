#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace btft {

class Environment final {
public:
    static Environment &get_instance() {
        static Environment env;
        return env;
    }

    void set_local(const std::string &name, const std::string &value);
    std::optional<std::string> get_local(const std::string &name) const;
    bool has_local(const std::string &name) const;

    void set_global(const std::string &name, const std::string &value);
    std::optional<std::string> get_global(const std::string &name) const;
    bool has_global(const std::string &name) const;

private:
    Environment() = default;

    std::unordered_map<std::string, std::string> local_vars;
    std::unordered_map<std::string, std::string> global_vars;
};

}  // namespace btft
