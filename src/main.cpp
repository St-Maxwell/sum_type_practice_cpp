#include <string>
#include <iostream>

#include <fmt/core.h>
#include <fmt/color.h>

#include "token.hpp"
#include "tokenize.hpp"
#include "parse.hpp"
#include "expr.hpp"
#include "evaluate.hpp"
#include "misc.hpp"

int main()
{
    fmt::print(fmt::fg(fmt::terminal_color::bright_red), "-- Type a piece of arithmetic expression\n");
    fmt::print(fmt::fg(fmt::terminal_color::bright_red), "-- Or use Ctrl-D to exit\n");

    std::string line;
    fmt::print(fmt::fg(fmt::color::steel_blue), ">>> ");
    while (std::getline(std::cin, line))
    {
        auto tokens = tokenize(line);
        if (!tokens)
            fmt::print(fmt::fg(fmt::color::red), "Error: {}\n", tokens.error());
        else
        {
            auto tree = parse(*tokens);
            if (!tree)
                fmt::print(fmt::fg(fmt::color::red), "Error: {}\n", tree.error());
            else
            {
                auto rpn_ops = translate(*tree);
                fmt::println("{}", evaluate(std::move(rpn_ops)));
            }
        }

        fmt::print(fmt::fg(fmt::color::steel_blue), ">>> ");
    }
    fmt::println("");
}
