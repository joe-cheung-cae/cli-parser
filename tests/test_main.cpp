#include <catch2/catch_test_macros.hpp>
#include <cli_parser.hpp>

TEST_CASE("ArgumentParser basic parse", "[parser]") {
    clipp::ArgumentParser parser("test_program");

    char* argv[] = {"test_program", "arg1", "arg2"};
    parser.parse(3, argv);

    auto args = parser.get_args();
    REQUIRE(args.size() == 3);
    REQUIRE(args[0] == "test_program");
    REQUIRE(args[1] == "arg1");
    REQUIRE(args[2] == "arg2");
}