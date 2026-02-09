#include "channel.h"
#include <exception>
#include <iostream>

void Channel::write(const std::string &buffer) {
    std::unique_lock<std::mutex> mutexWrite(mutex);
    if (closed) {
        throw std::runtime_error("Channel is closed, you can't write into it");
    }
    readBuffer += buffer;
    mutexWrite.unlock();

    condVar.notify_all();
}

std::string Channel::read() {
    std::unique_lock<std::mutex> mutexRead(mutex);
    condVar.wait(mutexRead, [this]() {
        return closed || readBuffer.size() != 0;
    });

    std::string result = std::move(readBuffer);
    readBuffer.clear();
    return result;
}

void Channel::closeChannel() {
    {
        std::unique_lock<std::mutex> mutexClose(mutex);
        closed = true;
    }
    condVar.notify_all();
}

bool Channel::isClosed() const noexcept {
    return closed;
}

std::string InputStdChannel::read() {
    std::string result;
    std::getline(std::cin, result);
    result += "\n";
    return result;
}

bool InputStdChannel::isClosed() const noexcept {
    return std::cin.eof();
}

void InputStdChannel::closeChannel() {
}

void OutputStdChannel::write(const std::string &buffer) {
    std::cout << buffer;
}

void OutputStdChannel::closeChannel() {
}