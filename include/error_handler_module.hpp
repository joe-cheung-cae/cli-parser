#pragma once

#include <string>
#include <iostream>

namespace clipp {

struct ParseResult {
    bool success = true;
    std::string error_message;
};

/**
 * @brief Handles and formats error messages for better user experience.
 * @param error_type Type of error (e.g., "parsing", "missing_value").
 * @param details Additional details about the error.
 * @return Formatted error message.
 */
std::string format_error_message(const std::string& error_type, const std::string& details) {
    std::string msg = "Error";
    if (!error_type.empty()) {
        msg += " (" + error_type + ")";
    }
    msg += ": " + details + ". Please check the usage and try again.";
    return msg;
}

/**
 * @brief Prints an error message to stderr.
 * @param result The ParseResult containing the error.
 */
void print_error(const ParseResult& result) {
    if (!result.success) {
        std::cerr << result.error_message << std::endl;
    }
}

} // namespace clipp