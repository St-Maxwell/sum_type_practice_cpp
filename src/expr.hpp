#pragma once

#include <variant>

#include "misc.hpp"
#include "token.hpp"
#include "evaluate.hpp"

struct literal_t
{
    double num;
};

using expr_t = std::variant<literal_t,
                            box<struct binary_op>,
                            box<struct unary_op>>;

struct binary_op
{
    expr_t left;
    token_op op;
    expr_t right;
};

struct unary_op
{
    token_op op;
    expr_t right;
};

std::vector<op_t> translate(const expr_t &expr);
