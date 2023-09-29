#include <cmath>
#include <stack>

#include "evaluate.hpp"
#include "misc.hpp"

double evaluate(const std::vector<op_t> &rpn_expr)
{
    std::stack<double> operand;

    for (auto op : rpn_expr)
    {
        // clang-format off
        match_variant(op,
            [&operand](double num) { operand.push(num); },
            [&operand](char c)
            {
                switch (c)
                {
                case '+':
                {
                    double last = operand.top(); operand.pop();
                    double first = operand.top(); operand.pop();
                    operand.push(first + last);
                    break;
                }
                case '-':
                {
                    double last = operand.top(); operand.pop();
                    double first = operand.top(); operand.pop();
                    operand.push(first - last);
                    break;
                }
                case '*':
                {
                    double last = operand.top(); operand.pop();
                    double first = operand.top(); operand.pop();
                    operand.push(first * last);
                    break;
                }
                case '/':
                {
                    double last = operand.top(); operand.pop();
                    double first = operand.top(); operand.pop();
                    operand.push(first / last);
                    break;
                }
                case '~':
                {
                    double last = operand.top(); operand.pop();
                    operand.push(-last);
                    break;
                }
                case '^':
                {
                    double last = operand.top(); operand.pop();
                    double first = operand.top(); operand.pop();
                    operand.push(std::pow(first, last));
                    break;
                }
                default:
                    break;
                }
            }
        );
        // clang-format on
    }
    return operand.top();
}
