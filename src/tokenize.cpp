#include <string_view>
#include <charconv>

#include <fmt/format.h>

#include "token.hpp"
#include "tokenize.hpp"

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

char peek(std::string_view string)
{
    return string.empty() ? '\0' : string[0];
}

char peek_next(std::string_view string)
{
    return string.size() < 2 ? '\0' : string[1];
}

tl::expected<double, std::string> number(std::string_view &line)
{
    const char *start = line.data();

    while (is_digit(peek(line)))
        line.remove_prefix(1);
    if (peek(line) == '.' && is_digit(peek_next(line)))
    {
        line.remove_prefix(1); // Consume the '.'
        while (is_digit(peek(line)))
            line.remove_prefix(1);
    }

    const char *current = line.data();

    double value;
    auto [ptr, ec] = std::from_chars(start, current, value);

    if (ec == std::errc())
        return value;
    else
        return tl::unexpected(fmt::format("Unable to parse number '{}'",
                                          std::string_view(start, current - start)));
}

tokens_result tokenize(std::string_view line)
{
    std::vector<token_t> result;

    while (!line.empty())
    {
        char c = peek(line);
        if (is_digit(c))
        {
            auto num_result = number(line);
            if (num_result)
                result.push_back(token_num{*num_result});
            else
                return tl::unexpected(num_result.error());
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        {
            result.push_back(token_op{c});
            line.remove_prefix(1);
        }
        else if (c == '(' || c == ')')
        {
            result.push_back(token_paren{c});
            line.remove_prefix(1);
        }
        else if (c == ' ' || c == '\n' || c == '\t')
        {
            line.remove_prefix(1);
        }
        else
        {
            return tokens_result(tl::unexpect, fmt::format("Unexpected character: '{}'", c));
        }
    }
    return result;
}
