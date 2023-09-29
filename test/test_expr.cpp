#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.hpp>
#include <fmt/format.h>

#include "expr.hpp"
#include "misc.hpp"

std::string to_string(const std::vector<op_t> &rpn_expr)
{
    std::string str;
    for (auto op : rpn_expr)
    {
        match_variant(op,
                      [&str](auto v)
                      { str.append(fmt::format("{} ", v)); });
    }
    return str;
}

TEST_CASE("Parse Expression Tree (Variant)")
{
    SUBCASE("case1")
    {
        // 1 + 2
        auto e = binary_op{
            literal_t{1.},
            token_op{'+'},
            literal_t{2.},
        };

        auto result = translate(e);

        REQUIRE(to_string(result) == "1 2 + ");
    }

    SUBCASE("case2")
    {
        // - (1 + 2) * 2
        auto e = unary_op{
            token_op{'-'},
            binary_op{
                binary_op{
                    literal_t{1.},
                    token_op{'+'},
                    literal_t{2.}},
                token_op{'*'},
                literal_t{2.}}};

        auto result = translate(e);

        REQUIRE(to_string(result) == "1 2 + 2 * ~ ");
    }
}
