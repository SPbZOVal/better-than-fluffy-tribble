#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace btft {

class Environment final {
public:
    static Environment &instance() {
        static Environment env;
        return env;
    }

    void set(std::string name, std::string value);
    std::optional<std::string> get(const std::string &name) const;
    bool has(const std::string &name) const;

private:
    Environment() = default;

    std::unordered_map<std::string, std::string> vars;
};

}  // namespace btft
