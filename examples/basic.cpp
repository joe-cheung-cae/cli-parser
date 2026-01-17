#include <cli_parser.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    clipp::ArgumentParser parser("Basic Example", "Demonstrates basic CLI parsing");

    parser.add_option<std::string>("-f", "--file", "Input file path", true);
    int default_count = 10;
    parser.add_option<int>("-n", "--count", "Number of items", false, &default_count);
    parser.add_option<std::vector<int>>("-v", "--values", "List of values");

    auto result = parser.parse(argc, argv);
    if (!result.success) {
        std::cerr << "Error: " << result.error_message << std::endl;
        parser.print_usage();
        return 1;
    }

    std::cout << "File: " << parser.get<std::string>("--file") << std::endl;
    std::cout << "Count: " << parser.get<int>("--count") << std::endl;

    auto values = parser.get<std::vector<int>>("--values");
    std::cout << "Values: ";
    for (int v : values) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}