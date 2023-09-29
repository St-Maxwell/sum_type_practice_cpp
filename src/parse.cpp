#include <vector>
#include <tuple>
#include <optional>

#include <fmt/format.h>
#include <expected.hpp>

#include "token.hpp"
#include "expr.hpp"
#include "misc.hpp"

using parse_result = tl::expected<std::tuple<expr_t, size_t>, std::string>;

parse_result parse_E(const std::vector<token_t> &tokens, size_t pos);
parse_result parse_T(const std::vector<token_t> &tokens, size_t pos);
parse_result parse_F(const std::vector<token_t> &tokens, size_t pos);
parse_result parse_P(const std::vector<token_t> &tokens, size_t pos);

#define PARSE(node, next_pos, parse_fn, tokens, pos) \
    auto result = parse_fn(tokens, pos);             \
    if (!result)                                     \
        return tl::unexpected(result.error());       \
    std::tie(node, next_pos) = std::move(*result);

std::optional<token_t> next_token(const std::vector<token_t> &tokens, size_t pos)
{
    if (pos < tokens.size())
        return tokens[pos];
    else
        return std::nullopt;
}

tl::expected<expr_t, std::string> parse(const std::vector<token_t> &tokens)
{
    auto result = parse_E(tokens, 0);
    if (!result)
        return tl::unexpected(result.error());

    auto [E, next_pos] = std::move(*result);

    if (next_pos == tokens.size())
        return E;
    else
        return tl::unexpected(fmt::format("Expect end of input, but found '{}'", tokens[next_pos]));
}

/**
 * E --> T {( "+" | "-" ) T}
 */
parse_result parse_E(const std::vector<token_t> &tokens, size_t pos)
{
    expr_t T;
    size_t next_pos;
    PARSE(T, next_pos, parse_T, tokens, pos);

    while (next_pos < tokens.size())
    {
        // clang-format off
        if (!match_variant(tokens[next_pos],
                [](token_op op) -> bool
                    { return op.op == '+' || op.op == '-'; },
                []([[maybe_unused]] auto any) -> bool
                    { return false; })
        ) break;
        // clang-format on

        auto op = std::get<token_op>(tokens[next_pos]);

        expr_t T1;
        PARSE(T1, next_pos, parse_T, tokens, next_pos + 1);

        T = binary_op{T, op, T1};
    }

    return std::make_tuple(T, next_pos);
}

/**
 * T --> F {( "*" | "/" ) F}
 */
parse_result parse_T(const std::vector<token_t> &tokens, size_t pos)
{
    expr_t F;
    size_t next_pos;
    PARSE(F, next_pos, parse_F, tokens, pos);

    while (next_pos < tokens.size())
    {
        // clang-format off
        if (!match_variant(tokens[next_pos],
                [](token_op op) -> bool
                    { return op.op == '*' || op.op == '/'; },
                []([[maybe_unused]] auto any) -> bool
                    { return false; })
        ) break;
        // clang-format on

        auto op = std::get<token_op>(tokens[next_pos]);

        expr_t F1;
        PARSE(F1, next_pos, parse_F, tokens, next_pos + 1);

        F = binary_op{F, op, F1};
    }

    return std::make_tuple(F, next_pos);
}

/**
 * F --> P ["^" F]
 */
parse_result parse_F(const std::vector<token_t> &tokens, size_t pos)
{
    expr_t P;
    size_t next_pos;
    PARSE(P, next_pos, parse_P, tokens, pos);

    auto next = next_token(tokens, next_pos);
    if (!next)
        return std::make_tuple(P, next_pos);

    // clang-format off
    if (match_variant(*next,
            [](token_op op) -> bool
                { return op.op == '^'; },
            []([[maybe_unused]] auto any) -> bool
                { return false; })
    ) // clang-format on
    {
        auto op = std::get<token_op>(*next);

        expr_t F;
        PARSE(F, next_pos, parse_F, tokens, next_pos + 1);

        P = binary_op{P, op, F};
        return std::make_tuple(P, next_pos);
    }
    else
    {
        return std::make_tuple(P, next_pos);
    }
}

/**
 * P --> v | "(" E ")" | "-" T
 */
parse_result parse_P(const std::vector<token_t> &tokens, size_t pos)
{
    auto next = next_token(tokens, pos);
    if (!next)
        return tl::unexpected("Unexpected end of input, expect a paren or number");

    // clang-format off
    if (match_variant(*next,
            []([[maybe_unused]] token_num num) -> bool
                { return true; },
            []([[maybe_unused]] auto any) -> bool
                { return false; })
    ) // clang-format on
    {
        expr_t leaf = literal_t{std::get<token_num>(*next).num};
        return std::make_tuple(leaf, pos + 1);
    }

    // clang-format off
    if (match_variant(*next,
            [](token_paren paren) -> bool
                { return paren.paren == '('; },
            []([[maybe_unused]] auto any) -> bool
                { return false; })
    ) // clang-format on
    {
        expr_t E;
        size_t next_pos;
        PARSE(E, next_pos, parse_E, tokens, pos + 1);

        next = next_token(tokens, next_pos);
        if (!next)
            return tl::unexpected("Unexpected end of input, expect a paren");

        // clang-format off
        if (!match_variant(*next,
                [](token_paren paren) -> bool
                    { return paren.paren == ')'; },
                []([[maybe_unused]] auto any) -> bool
                    { return false; })
        ) return tl::unexpected(fmt::format("Expect ')', but found '{}'", *next));
        // clang-format on

        return std::make_tuple(E, next_pos + 1);
    }

    // clang-format off
    if (match_variant(*next,
            [](token_op op) -> bool
                { return op.op == '-'; },
            []([[maybe_unused]] auto any) -> bool
                { return false; })
    ) // clang-format on
    {
        auto op = std::get<token_op>(*next);

        expr_t F;
        size_t next_pos;
        PARSE(F, next_pos, parse_F, tokens, pos + 1);

        F = unary_op{op, F};
        return std::make_tuple(F, next_pos);
    }

    return tl::unexpected(fmt::format("Unexpected token '{}', expected paren or number", *next));
}
