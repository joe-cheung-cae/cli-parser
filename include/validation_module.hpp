#pragma once

#include <string>
#include <stdexcept>

namespace clipp {

const size_t MAX_ARG_LENGTH = 1024; // Example max length

/**
 * @brief Validates an argument string for basic safety and format.
 * @param arg The argument string to validate.
 * @throws std::invalid_argument if validation fails.
 */
void validate_argument(const std::string& arg) {
    if (arg.empty()) {
        throw std::invalid_argument("Argument cannot be empty");
    }
    if (arg.size() > MAX_ARG_LENGTH) {
        throw std::invalid_argument("Argument too long");
    }
    // Add more validations as needed, e.g., regex for allowed characters
}

/**
 * @brief Validates that an index is within bounds for argv.
 * @param i Current index.
 * @param argc Total argument count.
 * @throws std::out_of_range if out of bounds.
 */
void validate_index(int i, int argc) {
    if (i >= argc) {
        throw std::out_of_range("Index out of bounds");
    }
}

} // namespace clipp