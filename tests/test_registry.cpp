#include "test_registry.h"

namespace btft::test {

TEST_F(RegistryTest, GetKnownCommandReturnsCorrectCommand) {
    // Arrange - Register test commands
    registry->registerCommand<interpreter::executor::commands::EchoCommand>(
        "echo"
    );
    registry->registerCommand<interpreter::executor::commands::CatCommand>("cat"
    );

    // Act
    auto echo_cmd = registry->getCommand("echo");
    auto cat_cmd = registry->getCommand("cat");

    // Assert
    EXPECT_NE(echo_cmd, nullptr);
    EXPECT_NE(cat_cmd, nullptr);

    // Test that they are different instances
    EXPECT_NE(echo_cmd.get(), cat_cmd.get());
}

TEST_F(RegistryTest, GetUnknownCommandReturnsExternalCommand) {
    // Act
    auto unknown_cmd = registry->getCommand("unknown_command");

    // Assert
    EXPECT_NE(unknown_cmd, nullptr);

    // The command should be an ExternalCommand (we can test by checking it
    // doesn't crash)
    auto result = unknown_cmd->Execute({}, input_channel, output_channel);
    // External command should return some result without crashing
    EXPECT_FALSE(result.should_exit);
}

TEST_F(RegistryTest, SameCommandNameReturnsSameInstance) {
    // Arrange
    registry->registerCommand<interpreter::executor::commands::EchoCommand>(
        "testcmd"
    );

    // Act
    auto cmd1 = registry->getCommand("testcmd");
    auto cmd2 = registry->getCommand("testcmd");

    // Assert
    EXPECT_NE(cmd1, nullptr);
    EXPECT_NE(cmd2, nullptr);
    EXPECT_EQ(cmd1.get(), cmd2.get());  // Should be the same instance
}

TEST_F(RegistryTest, CommandExecutionWorksThroughRegistry) {
    // Arrange
    registry->registerCommand<interpreter::executor::commands::EchoCommand>(
        "echo"
    );
    auto echo_cmd = registry->getCommand("echo");

    // Act
    auto result =
        echo_cmd->Execute({"hello", "world"}, input_channel, output_channel);

    // Assert
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_FALSE(result.should_exit);
    EXPECT_TRUE(result.error_message.empty());
    EXPECT_EQ(output_channel->get_output(), "hello world\n");
}

}  // namespace btft::test
