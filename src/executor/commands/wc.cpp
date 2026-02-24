#include <executor/commands/wc.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace btft::interpreter::executor::commands {

namespace {

std::tuple<std::size_t, std::size_t, std::size_t> CountStats(
    const std::string &content
) {
    std::size_t lines = 0;
    std::size_t words = 0;
    std::size_t bytes = content.size();

    bool in_word = false;
    for (char c : content) {
        if (c == '\n') {
            lines++;
        }
        if (std::isspace(static_cast<unsigned char>(c))) {
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

}  // namespace

ExecutionResult WcCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> inputChannel,
    std::shared_ptr<IOutputChannel> outputChannel
) {
    std::size_t total_lines = 0;
    std::size_t total_words = 0;
    std::size_t total_bytes = 0;

    if (args.empty()) {
        std::string content;
        while (true) {
            const std::string chunk = inputChannel->Read();
            if (chunk.empty() && inputChannel->IsClosed()) {
                break;
            }
            content += chunk;
        }

        const auto [lines, words, bytes] = CountStats(content);
        outputChannel->Write(
            std::to_string(lines) + " " + std::to_string(words) + " " +
            std::to_string(bytes) + "\n"
        );

        return ExecutionResult{};
    }

    for (const auto &filename : args) {
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

        outputChannel->Write(
            std::to_string(lines) + " " + std::to_string(words) + " " +
            std::to_string(bytes) + " " + filename + "\n"
        );
    }

    if (args.size() > 1) {
        outputChannel->Write(
            std::to_string(total_lines) + " " + std::to_string(total_words) +
            " " + std::to_string(total_bytes) + " total\n"
        );
    }

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
