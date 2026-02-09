#include "channel.h"
#include <exception>
#include <iostream>

Channel::Channel() : stream(std::stringstream::in | std::stringstream::out) {
}

void Channel::write(const std::string &buffer) {
    std::unique_lock<std::mutex> mutexWrite(mutex);
    if (closed) {
        throw std::runtime_error("Channel is closed, you can't write into it");
    }
    stream << buffer;
    mutexWrite.unlock();

    condVar.notify_one();
}

std::string Channel::read() {
    std::unique_lock<std::mutex> mutexRead(mutex);
    condVar.wait(mutexRead, [this]() {
        return closed || stream.str().size() != 0;
    });

    std::string result = std::move(stream.str());
    stream.clear();
    return result;
}

void Channel::closeChannel() {
    {
        std::unique_lock<std::mutex> mutexClose(mutex);
        closed = true;
    }
    condVar.notify_all();
}

bool Channel::isClosed() {
    return closed;
}

std::string InputChannel::read() {
    std::string result;
    std::getline(std::cin, result);
    return result;
}

void InputChannel::write(const std::string &bufffer) {
    throw std::runtime_error("Can't write to std::cin");
}

bool InputChannel::isClosed() {
    return std::cin.eof();
}

void InputChannel::closeChannel() {
    throw std::runtime_error("Can't close std::cin");
}

std::string OutputChannel::read() {
    throw std::runtime_error("Can't read from std::cin");
}

void OutputChannel::write(const std::string &buffer) {
    std::cout << buffer;
}

void OutputChannel::closeChannel() {
}

bool OutputChannel::isClosed() {
    return false;
}