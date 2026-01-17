# CLI Parser

A lightweight, header-only C++ library for parsing command-line arguments.

## Features

- Header-only implementation
- Type-safe parsing
- Support for short and long options
- Automatic usage generation
- Cross-platform

## Usage

```cpp
#include <cli_parser.hpp>

int main(int argc, char* argv[]) {
    clipp::ArgumentParser parser("My Program", "A sample CLI app");

    parser.add_option<std::string>("-f", "--file", "Input file", true);
    parser.add_option<int>("-n", "--number", "A number", false, 42);

    // Parse arguments
    auto result = parser.parse(argc, argv);
    if (!result.success) {
        std::cerr << result.error_message << std::endl;
        parser.print_usage();
        return 1;
    }

    auto file = parser.get<std::string>("--file");
    auto num = parser.get<int>("--number");

    std::cout << "File: " << file << ", Number: " << num << std::endl;
    return 0;
}
```

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Testing

```bash
./tests