#include "test_commands.h"
#include <filesystem>
#include <fstream>

namespace btft::test {

// CatCommand tests
TEST_F(CatCommandTest, CatWithNoArgumentsReadsFromInputChannel) {
    // Arrange
    std::vector<std::string> input_data = {"line1\n", "line2\n", "line3\n"};
    input_channel->set_input_data(input_data);

    // Act
    auto result = cat_command->Execute({}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
    EXPECT_EQ(output_channel->get_output(), "line1\nline2\nline3\n");
}

TEST_F(CatCommandTest, CatWithFileArgumentReadsFromFile) {
    // Arrange
    std::string test_filename = "test_cat_file.txt";
    std::string test_content = "Hello World\nThis is a test file\n";

    std::ofstream file(test_filename);
    file << test_content;
    file.close();

    // Act
    auto result = cat_command->Execute({test_filename}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
    EXPECT_EQ(output_channel->get_output(), test_content);

    // Cleanup
    std::filesystem::remove(test_filename);
}

TEST_F(CatCommandTest, CatWithNonExistentFileReturnsError) {
    // Act
    auto result = cat_command->Execute({"nonexistent_file.txt"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 1);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(CatCommandTest, CatWithMultipleFilesConcatenatesContent) {
    // Arrange
    std::string filename1 = "test_file1.txt";
    std::string filename2 = "test_file2.txt";
    std::string content1 = "File 1 content\n";
    std::string content2 = "File 2 content\n";

    std::ofstream file1(filename1);
    file1 << content1;
    file1.close();

    std::ofstream file2(filename2);
    file2 << content2;
    file2.close();

    // Act
    auto result = cat_command->Execute({filename1, filename2}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_EQ(output_channel->get_output(), content1 + content2);

    // Cleanup
    std::filesystem::remove(filename1);
    std::filesystem::remove(filename2);
}

// EchoCommand tests
TEST_F(EchoCommandTest, EchoWithNoArgumentsOutputsNewline) {
    // Act
    auto result = echo_command->Execute({}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
    EXPECT_EQ(output_channel->get_output(), "\n");
}

TEST_F(EchoCommandTest, EchoWithSingleArgument) {
    // Act
    auto result = echo_command->Execute({"hello"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_EQ(output_channel->get_output(), "hello\n");
}

TEST_F(EchoCommandTest, EchoWithMultipleArguments) {
    // Act
    auto result = echo_command->Execute({"hello", "world", "test"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_EQ(output_channel->get_output(), "hello world test\n");
}

TEST_F(EchoCommandTest, EchoIgnoresInputChannel) {
    // Arrange
    std::vector<std::string> input_data = {"this should be ignored\n"};
    input_channel->set_input_data(input_data);

    // Act
    auto result = echo_command->Execute({"test"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_EQ(output_channel->get_output(), "test\n");
}

// WcCommand tests
TEST_F(WcCommandTest, WcWithNoArgumentsCountsFromInputChannel) {
    // Arrange
    std::vector<std::string> input_data = {"Hello World\n", "This is a test\n"};
    input_channel->set_input_data(input_data);

    // Act
    auto result = wc_command->Execute({}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
    EXPECT_EQ(output_channel->get_output(), "2 6 26\n");
}

TEST_F(WcCommandTest, WcWithFileArgument) {
    // Arrange
    std::string test_filename = "test_wc_file.txt";
    std::string test_content = "Hello World\nThis is a test\n";

    std::ofstream file(test_filename);
    file << test_content;
    file.close();

    // Act
    auto result = wc_command->Execute({test_filename}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_EQ(output_channel->get_output(), "2 6 26 test_wc_file.txt\n");

    // Cleanup
    std::filesystem::remove(test_filename);
}

TEST_F(WcCommandTest, WcWithMultipleFiles) {
    // Arrange
    std::string filename1 = "test_wc1.txt";
    std::string filename2 = "test_wc2.txt";
    std::string content1 = "Hello\n";
    std::string content2 = "World\nTest\n";

    std::ofstream file1(filename1);
    file1 << content1;
    file1.close();

    std::ofstream file2(filename2);
    file2 << content2;
    file2.close();

    // Act
    auto result = wc_command->Execute({filename1, filename2}, input_channel, output_channel);

    // Assert
    std::string expected_output = "1 1 6 test_wc1.txt\n2 2 11 test_wc2.txt\n3 3 17 total\n";
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_EQ(output_channel->get_output(), expected_output);

    // Cleanup
    std::filesystem::remove(filename1);
    std::filesystem::remove(filename2);
}

TEST_F(WcCommandTest, WcWithNonExistentFileReturnsError) {
    // Act
    auto result = wc_command->Execute({"nonexistent_file.txt"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 1);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
}

// PwdCommand tests
TEST_F(PwdCommandTest, PwdOutputsCurrentDirectory) {
    // Act
    auto result = pwd_command->Execute({}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());

    std::filesystem::path expected_path = std::filesystem::current_path();
    EXPECT_EQ(output_channel->get_output(), expected_path.string() + "\n");
}

TEST_F(PwdCommandTest, PwdIgnoresArguments) {
    // Act
    auto result = pwd_command->Execute({"ignored", "arguments"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);

    std::filesystem::path expected_path = std::filesystem::current_path();
    EXPECT_EQ(output_channel->get_output(), expected_path.string() + "\n");
}

// ExitCommand tests
TEST_F(ExitCommandTest, ExitWithNoArgumentsSetsExitFlag) {
    // Act
    auto result = exit_command->Execute({}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_TRUE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(ExitCommandTest, ExitWithValidCode) {
    // Act
    auto result = exit_command->Execute({"42"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 42);
    EXPECT_TRUE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(ExitCommandTest, ExitWithInvalidCodeDefaultsToZero) {
    // Act
    auto result = exit_command->Execute({"invalid"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_TRUE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
}

}  // namespace btft::test
