#pragma once

#include <vector>
#include <variant>

// double stands for numbers
// char stands for operators
// '+', '-', '*', '/', '~'(unary minus operator), '^'
using op_t = std::variant<double, char>;

// evaluate an arithmetic expression in RPN
double evaluate(const std::vector<op_t> &rpn_expr);
