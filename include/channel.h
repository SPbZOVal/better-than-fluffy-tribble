#pragma once

#include <condition_variable>
#include <mutex>
#include <sstream>
#include <string>

namespace NInterpretator::NExecutor {
class IChannel {
public:
    virtual void closeChannel() = 0;
};

class IInputChannel : public IChannel {
public:
    virtual ~IInputChannel() = default;

    virtual std::string read() = 0;
    virtual bool isClosed() const noexcept = 0;
};

class IOutputChannel : public IChannel {
public:
    virtual ~IOutputChannel() = default;

    virtual void write(const std::string &buffer) = 0;
};

class InputStdChannel final : public IInputChannel {
public:
    std::string read() override;
    void closeChannel() override;
    bool isClosed() const noexcept override;
};

class OutputStdChannel final : public IOutputChannel {
public:
    void write(const std::string &buffer) override;
    void closeChannel() override;
};

class Channel final : virtual public IOutputChannel, public IInputChannel {
public:
    void write(const std::string &buffer) override;
    std::string read() override;
    void closeChannel() override;
    bool isClosed() const noexcept override;

private:
    std::mutex mutex;
    std::condition_variable condVar;
    std::string readBuffer;
    bool closed = false;
};
}  // namespace NInterpretator::NExecutor