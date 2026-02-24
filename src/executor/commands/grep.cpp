#include "executor/commands/grep.h"
#include <argparse/argparse.hpp>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string_view>

namespace {

struct GrepArguments {
    bool is_word;
    bool is_case_sensitive;
    std::uint32_t after_context;
    std::string pattern;
    std::vector<std::string> files;
};

GrepArguments ParseArguments(const std::vector<std::string> &args) {
    GrepArguments parsed_arguments;
    std::vector<std::string> args_with_prefix;
    args_with_prefix.push_back("grep");
    for (const auto &arg : args) {
        args_with_prefix.push_back(arg);
    }

    argparse::ArgumentParser parser("grep");

    parser.add_argument("pattern");
    parser.add_argument("files").remaining();
    parser.add_argument("-w").default_value(false).implicit_value(true);
    parser.add_argument("-i").default_value(false).implicit_value(true);
    parser.add_argument("-A").scan<'i', int>().default_value(0);

    try {
        parser.parse_args(args_with_prefix);
    } catch (const std::exception &e) {
        std::cerr << "grep: " << e.what() << "\n";
        throw;
    }

    parsed_arguments.pattern = parser.get<std::string>("pattern");

    if (parser.present("files")) {
        parsed_arguments.files = parser.get<std::vector<std::string>>("files");
    }

    parsed_arguments.is_word = parser.get<bool>("-w");
    parsed_arguments.is_case_sensitive = parser.get<bool>("-i");
    parsed_arguments.after_context =
        static_cast<std::uint32_t>(parser.get<int>("-A"));
    return parsed_arguments;
}

std::regex PrepareRegex(const GrepArguments &arguments) {
    std::string pattern = arguments.pattern;
    if (arguments.is_word) {
        pattern = "\\b(" + pattern + ")\\b";
    }

    std::regex::flag_type flags = std::regex::ECMAScript;
    if (!arguments.is_case_sensitive) {
        flags |= std::regex::icase;
    }

    return std::regex{std::move(pattern), flags};
}

std::optional<std::string>
FindMatchesInLine(std::string &line, std::regex &re) {
    static constexpr std::string ANSI_RED_START = "\033[1;31m";
    static constexpr std::string ANSI_RED_END = "\033[0m";
    std::stringstream result;
    std::smatch search_result;
    bool found_any_match = false;

    while (std::regex_search(line, search_result, re)) {
        std::size_t match_position = search_result.position();

        result << std::string_view{line.c_str(), match_position};
        result << ANSI_RED_START;
        result << search_result.str();
        result << ANSI_RED_END;

        line = std::move(search_result.suffix());
        found_any_match = true;
    }

    if (!found_any_match) {
        return std::nullopt;
    }

    result << line;

    return std::optional{result.str()};
}

}  // namespace

namespace btft::interpreter::executor::commands {

ExecutionResult GrepCommand::Execute(
    const std::vector<std::string> &args,
    std::shared_ptr<IInputChannel> input_channel,
    std::shared_ptr<IOutputChannel> output_channel
) {
    GrepArguments arguments = ParseArguments(args);
    std::uint32_t current_after_context = 0;
    std::regex re = PrepareRegex(arguments);
    bool was_any_file = false;
    for (const auto &file : arguments.files) {
        if (!std::filesystem::exists(file)) {
            continue;
        }

        was_any_file = true;

        std::ifstream input_stream(file);
        std::string line;
        while (std::getline(input_stream, line)) {
            if (const auto result = FindMatchesInLine(line, re);
                result.has_value()) {
                output_channel->Write(*result);
                output_channel->Write("\n");
                ++current_after_context;
                if (current_after_context > arguments.after_context) {
                    return ExecutionResult{
                        .exit_code = 0, .should_exit = 0, .error_message = ""
                    };
                }
            }
        }
    }

    if (!was_any_file) {
        return ExecutionResult{
            .exit_code = 1,
            .should_exit = 1,
            .error_message = "Files are not exists"
        };
    }

    return ExecutionResult{};
}

}  // namespace btft::interpreter::executor::commands
