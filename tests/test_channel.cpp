#include "test_channel.h"

namespace btft::test {

TEST_F(ChannelTest, WriteAndReadBasic) {
    // Arrange
    std::string test_data = "Hello World\n";

    // Act
    channel->write(test_data);
    std::string result = channel->read();

    // Assert
    EXPECT_EQ(result, test_data);
    EXPECT_FALSE(channel->isClosed());
}

TEST_F(ChannelTest, MultipleWritesAreConcatenated) {
    // Arrange
    std::string data1 = "Hello ";
    std::string data2 = "World\n";

    // Act
    channel->write(data1);
    channel->write(data2);
    std::string result = channel->read();

    // Assert
    EXPECT_EQ(result, data1 + data2);
}

TEST_F(ChannelTest, ReadBlocksUntilDataAvailable) {
    // Arrange
    std::string test_data = "Test data\n";
    std::string result;
    bool read_completed = false;

    // Start a thread that will read from the channel
    std::thread reader([&]() {
        result = channel->read();
        read_completed = true;
    });

    // Give the reader time to start and block
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Verify reader is blocked
    EXPECT_FALSE(read_completed);

    // Act - Write data to unblock the reader
    channel->write(test_data);

    // Wait for reader to complete
    reader.join();

    // Assert
    EXPECT_TRUE(read_completed);
    EXPECT_EQ(result, test_data);
}

TEST_F(ChannelTest, CloseChannelUnblocksReaders) {
    // Arrange
    bool read_completed = false;
    std::string result;

    // Start a thread that will read from the channel
    std::thread reader([&]() {
        result = channel->read();
        read_completed = true;
    });

    // Give the reader time to start and block
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Verify reader is blocked
    EXPECT_FALSE(read_completed);

    // Act - Close the channel
    channel->closeChannel();

    // Wait for reader to complete
    reader.join();

    // Assert
    EXPECT_TRUE(read_completed);
    EXPECT_TRUE(result.empty());
    EXPECT_TRUE(channel->isClosed());
}

TEST_F(ChannelTest, WriteAfterCloseThrowsException) {
    // Arrange
    channel->closeChannel();

    // Act & Assert
    EXPECT_THROW(channel->write("test"), std::runtime_error);
}

TEST_F(ChannelTest, ReadAfterCloseReturnsEmptyString) {
    // Arrange
    channel->closeChannel();

    // Act
    std::string result = channel->read();

    // Assert
    EXPECT_TRUE(result.empty());
    EXPECT_TRUE(channel->isClosed());
}

TEST_F(ChannelTest, ConcurrentReadWrite) {
    // Arrange
    const int num_writes = 100;
    std::string test_data = "data";
    std::vector<std::string> results;

    // Start a reader thread
    std::thread reader([&]() {
        for (int i = 0; i < num_writes; ++i) {
            results.push_back(channel->read());
        }
    });

    // Start a writer thread
    std::thread writer([&]() {
        for (int i = 0; i < num_writes; ++i) {
            channel->write(test_data + std::to_string(i) + "\n");
        }
    });

    // Wait for both threads to complete
    writer.join();
    reader.join();

    // Assert
    EXPECT_EQ(results.size(), num_writes);
    for (int i = 0; i < num_writes; ++i) {
        EXPECT_EQ(results[i], test_data + std::to_string(i) + "\n");
    }
}

}  // namespace btft::test
