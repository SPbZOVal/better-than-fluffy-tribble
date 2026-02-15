#pragma once

#include <unordered_map>
#include "icommand.h"

namespace btft::interpreter::executor {

class CommandsRegistry {
public:
    static CommandsRegistry &GetInstance() {
        static CommandsRegistry instance;
        return instance;
    }

    template <commands::DerivedFromICommand CommandType>
    void registerCommand(const std::string &name) {
        registry.emplace(name, CommandType::createCommand());
    }

    std::shared_ptr<commands::ICommand> getCommand(const std::string &name) {
        const auto commandIterator = registry.find(name);
        if (commandIterator == registry.end()) {
            return nullptr;  // TODO: return external command here
        }
        return commandIterator->second;
    }

private:
    CommandsRegistry() = default;

    CommandsRegistry(const CommandsRegistry &other) = delete;
    CommandsRegistry(CommandsRegistry &&other) = delete;

    CommandsRegistry &operator=(const CommandsRegistry &other) = delete;
    CommandsRegistry &operator=(CommandsRegistry &&other) = delete;

    std::unordered_map<std::string, std::shared_ptr<commands::ICommand>>
        registry;
};
}  // namespace btft::interpreter::executor
