Development Plan: Header-Only C++ Command Line Argument Parsing Library

1. Project Overview

The goal is to develop a lightweight, header-only, template-based C++ library for parsing command line arguments when invoking an executable from CLI.
The library must be:
	•	Cross-platform (Windows, Linux, macOS)
	•	Built with CMake integration support
	•	Header-only (no compiled binaries)
	•	Easy to integrate into existing C++ projects
	•	Type-safe and extensible

Target usage example:

my_program -j input.json --threads 8 --scale 0.25 --values 1 2 3 4

Where:
	•	-j and --json are aliases of the same argument
	•	Arguments are separated by spaces
	•	Parameters can be parsed into C++ native and STL types

⸻

2. Core Requirements
	1.	CMake Cross-Platform Support
	•	Provide a minimal CMake configuration for:
	•	Header-only usage via target_include_directories
	•	Installation and export as an INTERFACE library
	•	No platform-specific code except for standard C++.
	2.	Header-Only, Template-Based Implementation
	•	All functionality must be implemented in .hpp files.
	•	Use templates and type traits for type-safe parsing.
	•	No source (.cpp) files.
	3.	Automatic Usage / Help Message
	•	When:
	•	Required parameters are missing
	•	Unknown parameters are provided
	•	Type conversion fails
	•	The library must output:
	•	Program description
	•	List of parameters
	•	Short and long names
	•	Expected type
	•	Default values (if any)
	•	Example usage
	4.	Short and Long Option Support
	•	Each argument can have:
	•	A short form: -j
	•	A long form: --json
	•	Both map to the same internal parameter:

add_option("-j", "--json", "Input JSON file");


	5.	Space-Separated Parameter Format
	•	All parameters are separated by spaces:

--count 10 --scale 0.5 --name test

	•	No reliance on =:

--count=10   (not required to support)


	6.	Multi-Type Argument Parsing
Supported types:
	•	Scalar:
	•	int
	•	float
	•	double
	•	bool
	•	std::string
	•	Container:
	•	std::vector<T> where T ∈ {int, float, double, string}
Example:

--values 1 2 3 4 5


	7.	Modern C++ Style
	•	Minimum standard: C++17
	•	Use:
	•	std::optional
	•	std::variant
	•	std::string_view
	•	Type traits and SFINAE / concepts (if C++20 is enabled optionally)

⸻

3. Architecture Design

3.1 Main Components

Component	Description
Argument	Stores metadata for one CLI option
ArgumentParser	Central class handling registration and parsing
TypeConverter<T>	Template struct for converting strings to type T
UsageFormatter	Responsible for formatting help/usage text
ParseResult	Holds success/failure status and error message


⸻

3.2 Data Model

struct ArgumentBase {
    std::string short_name;
    std::string long_name;
    std::string description;
    bool required;
};

template<typename T>
struct Argument : ArgumentBase {
    T value;
    std::optional<T> default_value;
};


⸻

4. Parsing Strategy
	1.	Read argv[] into a token list
	2.	Iterate through tokens:
	•	Detect options starting with - or --
	•	Match with registered arguments
	3.	For each argument:
	•	Fetch required number of tokens:
	•	1 token for scalar types
	•	N tokens until next option for std::vector<T>
	4.	Convert tokens using TypeConverter<T>
	5.	On failure:
	•	Print error message
	•	Print usage
	•	Exit or throw exception (configurable)

⸻

5. Type Conversion System

Implement a generic converter:

template<typename T>
struct TypeConverter {
    static bool convert(const std::string& token, T& out);
};

Specializations:
	•	int
	•	float
	•	double
	•	bool
	•	std::string
	•	std::vector<T>

This ensures:
	•	Type safety
	•	Clear error reporting
	•	Easy extensibility

⸻

6. API Design

Example user API:

ArgumentParser parser("My Program", "CLI argument parsing example");

parser.add_option<std::string>("-j", "--json", "Input JSON file", true);
parser.add_option<int>("-t", "--threads", "Thread count", false, 4);
parser.add_option<double>("", "--scale", "Scaling factor", false, 1.0);
parser.add_option<std::vector<int>>("", "--values", "List of values");

parser.parse(argc, argv);

auto json_file = parser.get<std::string>("--json");
auto threads   = parser.get<int>("--threads");
auto values    = parser.get<std::vector<int>>("--values");


⸻

7. Usage Output Format

Example:

Usage:
  my_program [options]

Options:
  -j, --json <string>      Input JSON file (required)
  -t, --threads <int>      Thread count (default: 4)
      --scale <double>     Scaling factor (default: 1.0)
      --values <int...>    List of values
  -h, --help               Show this help message


⸻

8. CMake Integration Plan

Provide a minimal header-only interface:

add_library(cli_parser INTERFACE)
target_include_directories(cli_parser INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

Usage:

target_link_libraries(my_program PRIVATE cli_parser)


⸻

9. Development Phases

Phase 1 – Core Framework
	•	Argument registration
	•	Token scanning
	•	Short/long option matching

Phase 2 – Type System
	•	Scalar converters
	•	std::vector<T> support
	•	Error handling for invalid input

Phase 3 – Usage / Help Formatter
	•	Automatic formatting
	•	Required/default flag rendering
	•	Example generation

Phase 4 – Robustness
	•	Unknown option detection
	•	Missing value detection
	•	Repeated argument handling

Phase 5 – CMake + Examples
	•	Provide:
	•	Sample project
	•	Sample CMakeLists.txt
	•	Demo executable

⸻

10. Testing Strategy
	•	Unit tests:
	•	Scalar parsing
	•	Vector parsing
	•	Alias matching (-j vs --json)
	•	Error cases
	•	Platform tests:
	•	Windows (MSVC)
	•	Linux (GCC / Clang)
	•	macOS (Clang)

⸻

11. Deliverables
	1.	include/cli_parser.hpp
	2.	examples/basic.cpp
	3.	CMakeLists.txt
	4.	README:
	•	Installation
	•	API reference
	•	Usage examples
	•	Error handling examples

⸻

12. Future Extensions
	•	Support for:
	•	Subcommands (git clone, git pull)
	•	Optional --arg=value syntax
	•	Environment variable fallback
	•	JSON/YAML configuration file integration
	•	C++20 Concepts-based type constraints

⸻

This plan ensures your CLI argument parsing library is:
	•	Cross-platform
	•	Header-only
	•	Type-safe
	•	User-friendly
	•	Suitable for modern C++ project infrastructure