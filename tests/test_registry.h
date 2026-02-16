#pragma once

#include <gtest/gtest.h>
#include <memory>
#include "test_mock_channels.h"
#include "../include/executor/commands/registry.h"
#include "../include/executor/commands/cat.h"
#include "../include/executor/commands/echo.h"
#include "../include/executor/commands/wc.h"
#include "../include/executor/commands/pwd.h"
#include "../include/executor/commands/exit.h"

namespace btft::test {

class RegistryTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = &interpreter::executor::CommandsRegistry::GetInstance();
        input_channel = std::make_shared<MockInputChannel>();
        output_channel = std::make_shared<MockOutputChannel>();
    }

    void TearDown() override {
        input_channel->closeChannel();
        output_channel->closeChannel();
    }

    interpreter::executor::CommandsRegistry* registry;
    std::shared_ptr<MockInputChannel> input_channel;
    std::shared_ptr<MockOutputChannel> output_channel;
};

}  // namespace btft::test
