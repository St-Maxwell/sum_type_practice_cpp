#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.hpp"
#include <fmt/format.h>

#include "evaluate.hpp"

TEST_CASE("Evaluate Postfix")
{
    SUBCASE("case1")
    {
        // infix: 5 + ((1 + 2) * 4) - 3
        // postfix: 5 1 2 + 4 * + 3 -
        // result: 14
        std::vector<op_t> expr = {
            op_t{5.},
            op_t{1.},
            op_t{2.},
            op_t{'+'},
            op_t{4.},
            op_t{'*'},
            op_t{'+'},
            op_t{3.},
            op_t{'-'}};
        REQUIRE(evaluate(expr) == doctest::Approx(14.).epsilon(1e-10));
    }
    SUBCASE("case2")
    {
        // infix: 3 + 4 * 2 / (1 - 5)^3
        // postfix: 3 4 2 * 1 5 - 3 ^ / +
        // result: 2.875
        std::vector<op_t> expr = {
            op_t{3.},
            op_t{4.},
            op_t{2.},
            op_t{'*'},
            op_t{1.},
            op_t{5.},
            op_t{'-'},
            op_t{3.},
            op_t{'^'},
            op_t{'/'},
            op_t{'+'}};
        REQUIRE(evaluate(expr) == doctest::Approx(2.875).epsilon(1e-10));
    }
}
