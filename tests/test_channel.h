#pragma once

#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "../include/executor/channel.h"

namespace btft::test {

class ChannelTest : public ::testing::Test {
protected:
    void SetUp() override {
        channel = std::make_shared<interpreter::executor::Channel>();
    }

    void TearDown() override {
        channel->closeChannel();
    }

    std::shared_ptr<interpreter::executor::Channel> channel;
};

}  // namespace btft::test
