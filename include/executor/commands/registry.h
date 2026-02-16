#pragma once

#include <unordered_map>
#include "executor/commands/external.h"
#include "icommand.h"

namespace btft::interpreter::executor {

class CommandsRegistry {
public:
    static CommandsRegistry &GetInstance() {
        static CommandsRegistry instance;
        return instance;
    }

    template <commands::DerivedFromICommand CommandType>
    void RegisterCommand(const std::string &name) {
        registry.emplace(name, CommandType::CreateCommand());
    }

    std::shared_ptr<commands::ICommand> GetCommand(const std::string &name) {
        const auto command_iterator = registry.find(name);
        if (command_iterator == registry.end()) {
            // Return external command for unknown commands
            return std::make_shared<commands::ExternalCommand>();
        }
        return command_iterator->second;
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
