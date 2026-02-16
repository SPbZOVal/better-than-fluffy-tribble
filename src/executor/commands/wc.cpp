#include <executor/commands/wc.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace btft::interpreter::executor::commands {

namespace {

std::tuple<size_t, size_t, size_t> CountStats(const std::string& content) {
    size_t lines = 0;
    size_t words = 0;
    size_t bytes = content.size();

    bool in_word = false;
    for (char c : content) {
        if (c == '\n') {
            lines++;
        }
        if (std::isspace(c)) {
            if (in_word) {
                words++;
                in_word = false;
            }
        } else {
            in_word = true;
        }
    }
    if (in_word) {
        words++;
    }

    return {lines, words, bytes};
}

} // namespace

ExecutionResult WcCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    size_t total_lines = 0;
    size_t total_words = 0;
    size_t total_bytes = 0;

    if (args.empty()) {
        std::string content;
        while (!inputChannel->isClosed()) {
            content += inputChannel->read();
        }

        auto [lines, words, bytes] = CountStats(content);
        outputChannel->write(std::to_string(lines) + " " +
                            std::to_string(words) + " " +
                            std::to_string(bytes) + "\n");

        return ExecutionResult{};
    }

    for (const auto& filename : args) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return ExecutionResult{.exit_code = 1};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();

        auto [lines, words, bytes] = CountStats(content);
        total_lines += lines;
        total_words += words;
        total_bytes += bytes;

        outputChannel->write(std::to_string(lines) + " " +
                            std::to_string(words) + " " +
                            std::to_string(bytes) + " " +
                            filename + "\n");
    }

    if (args.size() > 1) {
        outputChannel->write(std::to_string(total_lines) + " " +
                            std::to_string(total_words) + " " +
                            std::to_string(total_bytes) + " total\n");
    }

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
