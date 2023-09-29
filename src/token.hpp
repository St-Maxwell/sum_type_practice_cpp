#pragma once

#include <variant>
#include <fmt/core.h>
#include <fmt/format.h>

#include "misc.hpp"

/**
 * '+', '-', '*', '/', '^'
 */
struct token_op
{
    char op;
};

/**
 * '(', ')'
 */
struct token_paren
{
    char paren;
};

struct token_num
{
    double num;
};

using token_t = std::variant<token_op, token_paren, token_num>;

template <>
struct fmt::formatter<token_t>
{
    auto format(const token_t &token, format_context &ctx) const
    {
        // clang-format off
        return match_variant(token,
            [&ctx](token_op op) -> format_context::iterator
                { return fmt::format_to(ctx.out(), "{}", op.op); },
            [&ctx](token_paren paren) -> format_context::iterator
                { return fmt::format_to(ctx.out(), "{}", paren.paren); },
            [&ctx](token_num num) -> format_context::iterator
                { return fmt::format_to(ctx.out(), "num({})", num.num); }
        );
        // clang-format on
    }

    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator
    {
        return ctx.begin();
    }
};
