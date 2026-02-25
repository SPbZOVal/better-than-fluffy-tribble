#pragma once

#include <condition_variable>
#include <mutex>
#include <sstream>
#include <string>

namespace btft::interpreter::executor {

class IChannel {
public:
    virtual ~IChannel() = default;
    virtual void CloseChannel() = 0;
};

class IInputChannel : public IChannel {
public:
    virtual ~IInputChannel() = default;

    virtual std::string Read() = 0;
    virtual bool IsClosed() const = 0;
};

class IOutputChannel : public IChannel {
public:
    virtual ~IOutputChannel() = default;

    virtual void Write(const std::string &buffer) = 0;
};

class InputStdChannel final : public IInputChannel {
public:
    std::string Read() override;
    void CloseChannel() override;
    bool IsClosed() const override;
};

class OutputStdChannel final : public IOutputChannel {
public:
    void Write(const std::string &buffer) override;
    void CloseChannel() override;
};

class Channel final : virtual public IOutputChannel, public IInputChannel {
public:
    void Write(const std::string &buffer) override;
    std::string Read() override;
    void CloseChannel() override;
    bool IsClosed() const override;

private:
    mutable std::mutex mutex;
    std::condition_variable condVar;
    std::stringstream readBuffer;
    bool closed = false;
};

}  // namespace btft::interpreter::executor
