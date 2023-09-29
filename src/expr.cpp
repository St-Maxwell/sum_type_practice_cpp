#include "expr.hpp"

void translate_expr(const expr_t &expr, std::vector<op_t> &rpn_expr)
{
    // clang-format off
    match_variant(expr,
        [&rpn_expr](const literal_t &num)
        {
            rpn_expr.push_back(num.num);
        },
        [&rpn_expr](const box<binary_op>& op)
        {
            translate_expr(op->left, rpn_expr);
            translate_expr(op->right, rpn_expr);
            rpn_expr.push_back(op->op.op);
        },
        [&rpn_expr](const box<unary_op>& op)
        {
            translate_expr(op->right, rpn_expr);
            if (op->op.op == '-')
                rpn_expr.push_back('~');
            else
                rpn_expr.push_back(op->op.op);
        }
    );
    // clang-format on
}

std::vector<op_t> translate(const expr_t &expr)
{
    std::vector<op_t> rpn_expr;
    translate_expr(expr, rpn_expr);
    return rpn_expr;
}
