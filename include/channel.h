#pragma once

#include <condition_variable>
#include <mutex>
#include <sstream>
#include <string>

class IChannel {
public:
    virtual ~IChannel() = default;

    virtual void write(const std::string &buffer) = 0;
    virtual std::string read() = 0;
    virtual void closeChannel() = 0;
    virtual bool isClosed() = 0;
};

class InputChannel final : public IChannel {
public:
    void write(const std::string &buffer) override;
    std::string read() override;
    void closeChannel() override;
    bool isClosed() override;
};

class OutputChannel final : public IChannel {
public:
    void write(const std::string &buffer) override;
    std::string read() override;
    void closeChannel() override;
    bool isClosed() override;
};

class Channel final : public IChannel {
public:
    Channel();
    void write(const std::string &buffer) override;
    std::string read() override;
    void closeChannel() override;
    bool isClosed() override;

private:
    std::mutex mutex;
    std::condition_variable condVar;
    std::stringstream stream;
    bool closed = false;
};