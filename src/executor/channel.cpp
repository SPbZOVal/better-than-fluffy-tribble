#include "executor/channel.h"
#include <iostream>

namespace btft::interpreter::executor {

void Channel::Write(const std::string &buffer) {
    std::unique_lock mutex_write(mutex);
    if (closed) {
        throw std::runtime_error("Channel is closed, you can't write into it");
    }
    readBuffer << buffer;
    condVar.notify_all();
}

std::string Channel::Read() {
    std::unique_lock mutex_read(mutex);
    condVar.wait(mutex_read, [this]() {
        return closed || readBuffer.str().size() != 0;
    });

    std::string result = std::move(readBuffer.str());
    readBuffer.str("");
    return result;
}

void Channel::CloseChannel() {
    std::unique_lock mutex_close(mutex);
    closed = true;
    condVar.notify_all();
}

bool Channel::IsClosed() const noexcept {
    return closed;
}

std::string InputStdChannel::Read() {
    std::string result;
    std::getline(std::cin, result);
    result += "\n";
    return result;
}

bool InputStdChannel::IsClosed() const noexcept {
    return std::cin.eof();
}

void InputStdChannel::CloseChannel() {
}

void OutputStdChannel::Write(const std::string &buffer) {
    std::cout << buffer;
}

void OutputStdChannel::CloseChannel() {
}

}  // namespace btft::interpreter::executor
