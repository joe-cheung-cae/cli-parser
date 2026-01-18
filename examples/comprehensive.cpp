#include <cli_parser.hpp>
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]) {
    clipp::ArgumentParser parser("Comprehensive Example", "Demonstrates various CLI argument types and features");

    // Required string option
    parser.add_option<std::string>("-f", "--file", "Input file path", true);

    // Optional int with default
    int default_count = 10;
    parser.add_option<int>("-n", "--count", "Number of items", false, &default_count);

    // Optional float
    float default_rate = 1.0f;
    parser.add_option<float>("-r", "--rate", "Processing rate", false, &default_rate);

    // Optional double
    double default_threshold = 0.5;
    parser.add_option<double>("-t", "--threshold", "Threshold value", false, &default_threshold);

    // Optional boolean
    bool default_verbose = false;
    parser.add_option<bool>("-v", "--verbose", "Enable verbose output", false, &default_verbose);

    // Optional vector of integers
    parser.add_option<std::vector<int>>("-i", "--integers", "List of integers (comma-separated)");

    // Optional vector of strings
    parser.add_option<std::vector<std::string>>("-s", "--strings", "List of strings (comma-separated)");

    auto result = parser.parse(argc, argv);
    if (!result.success) {
        std::cerr << "Error: " << result.error_message << std::endl;
        parser.print_usage();
        return 1;
    }

    // Display parsed values
    std::cout << "Parsed Arguments:" << std::endl;
    std::cout << "File: " << parser.get<std::string>("--file") << std::endl;
    std::cout << "Count: " << parser.get<int>("--count") << std::endl;
    std::cout << "Rate: " << std::fixed << std::setprecision(2) << parser.get<float>("--rate") << std::endl;
    std::cout << "Threshold: " << std::fixed << std::setprecision(4) << parser.get<double>("--threshold") << std::endl;
    std::cout << "Verbose: " << (parser.get<bool>("--verbose") ? "true" : "false") << std::endl;

    // Handle vectors (they might be empty)
    try {
        auto integers = parser.get<std::vector<int>>("--integers");
        if (!integers.empty()) {
            std::cout << "Integers: ";
            for (size_t i = 0; i < integers.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << integers[i];
            }
            std::cout << std::endl;
        }
    } catch (const std::runtime_error&) {
        // Option not provided, skip
    }

    try {
        auto strings = parser.get<std::vector<std::string>>("--strings");
        if (!strings.empty()) {
            std::cout << "Strings: ";
            for (size_t i = 0; i < strings.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << "\"" << strings[i] << "\"";
            }
            std::cout << std::endl;
        }
    } catch (const std::runtime_error&) {
        // Option not provided, skip
    }

    std::cout << "\nExample usage: " << argv[0] << " -f input.txt -n 5 -r 2.5 -t 0.75 -v true -i 1,2,3,4 -s hello,world,test" << std::endl;

    return 0;
}