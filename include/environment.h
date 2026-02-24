#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace btft {

class Environment final {
public:
    static Environment &GetInstance() {
        static Environment env;
        return env;
    }

    void SetLocal(const std::string &name, const std::string &value);
    std::optional<std::string> GetLocal(const std::string &name) const;
    bool HasLocal(const std::string &name) const;
    void ClearLocal();

    void SetGlobal(const std::string &name, const std::string &value);
    std::optional<std::string> GetGlobal(const std::string &name) const;
    bool HasGlobal(const std::string &name) const;

    bool HasVar(const std::string &name) const;
    std::optional<std::string> GetVar(const std::string &name) const;

private:
    Environment() = default;

    std::unordered_map<std::string, std::string> local_vars{};
    std::unordered_map<std::string, std::string> global_vars{};
};

}  // namespace btft
