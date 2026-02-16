#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "../include/executor/channel.h"

namespace btft::test {

/**
 * MockInputChannel - test implementation of IInputChannel
 *
 * This class allows pre-populating input data for testing commands
 * that read from input channels.
 */
class MockInputChannel final : public interpreter::executor::IInputChannel {
public:
    MockInputChannel() = default;

    // Set the input data that will be returned by read()
    void set_input_data(const std::vector<std::string>& data);

    std::string read() override;
    void closeChannel() override;
    bool isClosed() const noexcept override;

private:
    mutable std::mutex mutex_;
    std::vector<std::string> input_data_;
    size_t current_index_ = 0;
    bool closed_ = false;
};

/**
 * MockOutputChannel - test implementation of IOutputChannel
 *
 * This class captures output data for verification in tests.
 */
class MockOutputChannel final : public interpreter::executor::IOutputChannel {
public:
    MockOutputChannel() = default;

    void write(const std::string& buffer) override;
    void closeChannel() override;

    // Get the accumulated output data
    std::string get_output() const;

    // Clear the output buffer
    void clear();

    // Check if the channel is closed
    bool is_closed() const;

private:
    mutable std::mutex mutex_;
    std::string output_data_;
    bool closed_ = false;
};

}  // namespace btft::test
