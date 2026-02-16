#pragma once

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "../include/executor/commands/cat.h"
#include "../include/executor/commands/echo.h"
#include "../include/executor/commands/exit.h"
#include "../include/executor/commands/pwd.h"
#include "../include/executor/commands/wc.h"
#include "test_mock_channels.h"

namespace btft::test {

class CommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        input_channel = std::make_shared<MockInputChannel>();
        output_channel = std::make_shared<MockOutputChannel>();
    }

    void TearDown() override {
        input_channel->closeChannel();
        output_channel->closeChannel();
    }

    std::shared_ptr<MockInputChannel> input_channel;
    std::shared_ptr<MockOutputChannel> output_channel;
};

class CatCommandTest : public CommandTest {
protected:
    void SetUp() override {
        CommandTest::SetUp();
        cat_command =
            std::make_shared<interpreter::executor::commands::CatCommand>();
    }

    std::shared_ptr<interpreter::executor::commands::CatCommand> cat_command;
};

class EchoCommandTest : public CommandTest {
protected:
    void SetUp() override {
        CommandTest::SetUp();
        echo_command =
            std::make_shared<interpreter::executor::commands::EchoCommand>();
    }

    std::shared_ptr<interpreter::executor::commands::EchoCommand> echo_command;
};

class WcCommandTest : public CommandTest {
protected:
    void SetUp() override {
        CommandTest::SetUp();
        wc_command =
            std::make_shared<interpreter::executor::commands::WcCommand>();
    }

    std::shared_ptr<interpreter::executor::commands::WcCommand> wc_command;
};

class PwdCommandTest : public CommandTest {
protected:
    void SetUp() override {
        CommandTest::SetUp();
        pwd_command =
            std::make_shared<interpreter::executor::commands::PwdCommand>();
    }

    std::shared_ptr<interpreter::executor::commands::PwdCommand> pwd_command;
};

class ExitCommandTest : public CommandTest {
protected:
    void SetUp() override {
        CommandTest::SetUp();
        exit_command =
            std::make_shared<interpreter::executor::commands::ExitCommand>();
    }

    std::shared_ptr<interpreter::executor::commands::ExitCommand> exit_command;
};

}  // namespace btft::test
