#pragma once
#include <string_view>
#include <optional>
#include <stdexcept>
#include <cstddef>

struct ParsedLine {
    std::string_view cmd;
    std::optional<std::string_view> val;
};

// trim helper
std::string_view trim(std::string_view s) {
    const std::string_view whitespace = " \t";

    size_t start = s.find_first_not_of(whitespace);
    if (start == std::string_view::npos) return {};

    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}

std::optional<ParsedLine> parseLine(std::string_view line) {
    // find comment
    size_t comment_idx = line.find('#');
    if (comment_idx == std::string_view::npos)
        comment_idx = line.size();

    // slice + trim
    std::string_view clean_line = line.substr(0, comment_idx);
    std::string_view trimmed = trim(clean_line);

    if (trimmed.empty())
        return std::nullopt; // like `return null`

    // split manually (like Zig iterator)
    size_t comma = trimmed.find(',');

    std::string_view cmd;
    std::optional<std::string_view> val;

    if (comma == std::string_view::npos) {
        cmd = trimmed;
    } else {
        cmd = trimmed.substr(0, comma);
        val = trimmed.substr(comma + 1);
    }

    // trim parts
    cmd = trim(cmd);
    if (val.has_value()) {
        val = trim(*val);
    }

    if (cmd.empty()) {
        throw std::runtime_error("InvalidSyntax");
    }

    return ParsedLine{
        .cmd = cmd,
        .val = val
    };
}
