#include "test_mock_channels.h"

namespace btft::test {

// MockInputChannel implementation
void MockInputChannel::set_input_data(const std::vector<std::string> &data) {
    std::lock_guard lock(mutex_);
    input_data_ = data;
    current_index_ = 0;
    closed_ = false;
}

std::string MockInputChannel::read() {
    std::unique_lock lock(mutex_);
    if (closed_ || current_index_ >= input_data_.size()) {
        closed_ = true;
        return "";
    }

    std::string result = input_data_[current_index_];
    current_index_++;

    // If we've read all data, mark as closed
    if (current_index_ >= input_data_.size()) {
        closed_ = true;
    }

    return result;
}

void MockInputChannel::closeChannel() {
    std::lock_guard lock(mutex_);
    closed_ = true;
}

bool MockInputChannel::isClosed() const noexcept {
    std::lock_guard lock(mutex_);
    return closed_;
}

// MockOutputChannel implementation
void MockOutputChannel::write(const std::string &buffer) {
    std::lock_guard lock(mutex_);
    output_data_ += buffer;
}

void MockOutputChannel::closeChannel() {
    std::lock_guard lock(mutex_);
    closed_ = true;
}

std::string MockOutputChannel::get_output() const {
    std::lock_guard lock(mutex_);
    return output_data_;
}

void MockOutputChannel::clear() {
    std::lock_guard lock(mutex_);
    output_data_.clear();
}

bool MockOutputChannel::is_closed() const {
    std::lock_guard lock(mutex_);
    return closed_;
}

}  // namespace btft::test
