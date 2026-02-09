#pragma once

#include <unordered_map>
#include "commands/icommand.h"

class CommandsRegistry {
public:
    static CommandsRegistry &GetInstance() {
        static CommandsRegistry instance;
        return instance;
    }

    void
    registerCommand(const std::string &name, CommandCreator commandCreator) {
        registry.emplace(name, commandCreator());
    }

    std::shared_ptr<ICommand> getCommand(const std::string &name) {
        auto commandIterator = registry.find(name);
        if (commandIterator == registry.end()) {
            return nullptr;
        }
        return commandIterator->second;
    }

private:
    CommandsRegistry() = default;
    CommandsRegistry(const CommandsRegistry &other) = delete;
    CommandsRegistry(CommandsRegistry &&other) = delete;

    CommandsRegistry &operator=(const CommandsRegistry &other) = delete;
    CommandsRegistry &operator=(CommandsRegistry &&other) = delete;

    std::unordered_map<std::string, std::shared_ptr<ICommand>> registry;
};
