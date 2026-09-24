#include "parser.h"

void Parser::lexer() {

    while (peek()) {

        std::string lex;
        lex.reserve(32);

        switch (peek()) {

        case '+':
            tokens.emplace_back(Type::plus_operator, Precedence::PLUS);
            advance();
            break;
        case '-':
            tokens.emplace_back(Type::sub_operator, Precedence::SUB);
            advance();
            break;
        case '*':
            tokens.emplace_back(Type::mul_operator, Precedence::MUL);
            advance();
            break;
        case '/':
            tokens.emplace_back(Type::div_operator, Precedence::DIV);
            advance();
            break;
        case 'p':
            tokens.emplace_back(Type::percent_operator, Precedence::PER);
            advance();
            break;

        case ' ': case '\t': case '\r':
            advance();
            break;

        default:
            if (isdigit(peek())) {

                while (isdigit(peek())) {

                    lex += peek();
                    advance();

                }

                if (peek() == '.') {
                    lex += peek();
                    advance();
                    while (isdigit(peek())) {
                        lex += peek();
                        advance();
                    }

                }

                tokens.emplace_back(Type::operand, strtod(lex.c_str(), NULL));

            }
            else {

                SetWindowTextW(edit, L"invalid character");
                return;
            }

            break;

        };

    }

    parser();

}

void Parser::parser() {

    ParserState state = ParserState::EXPECT_OPERAND;

    for (auto& t : tokens) {

        switch (state) {

        case ParserState::EXPECT_OPERAND:
            if (t.type == Type::sub_operator) {
                if (!stack.empty() && stack.top().type == Type::unary_sub) {

                    SetWindowTextW(edit, L"two - - not allowed");
                    return;
                }

                t.type = Type::unary_sub;
                t.data = (int)Precedence::UNARY;
                stack.push(t);
            }

            else if (t.type == Type::operand) {

                operand.push(std::get<double>(t.data));
                state = ParserState::EXPECT_OPERATOR;

            }

            else {
                SetWindowTextW(edit, L"number needed");
                return;
            }

            break;

        case ParserState::EXPECT_OPERATOR:

            if (t.type >= Type::plus_operator && t.type <= Type::percent_operator) {

                // check precedence
                while (!stack.empty() && std::get<int>(stack.top().data) >= std::get<int>(t.data)) {
                    eval();
                }

                stack.push(t);

                state = ParserState::EXPECT_OPERAND;
            }

            else {
                SetWindowTextW(edit, L"Operator needed");
                return;

            }

            break;
        }

    }

    /* last token type check  */
    if (state != ParserState::EXPECT_OPERATOR) {

        SetWindowTextW(edit, L"Expression must end");
        return;
    };

    while (!stack.empty()) {
        eval();
    }

    // print result
    SetWindowTextW(edit, std::to_wstring(operand.top()).c_str());

}

void Parser::eval() {

    double a = 0.0, b = 0.0;

    switch (stack.top().type) {

    case Type::plus_operator:
        b = operand.top();
        operand.pop();
        a = operand.top();
        operand.pop();
        operand.push(a + b);
        break;
    case Type::sub_operator:
        b = operand.top();
        operand.pop();
        a = operand.top();
        operand.pop();
        operand.push(a - b);
        break;
    case Type::unary_sub:
        b = operand.top();
        operand.pop();
        operand.push(-b);
        break;
    case Type::mul_operator:
        b = operand.top();
        operand.pop();
        a = operand.top();
        operand.pop();
        operand.push(a * b);
        break;
    case Type::div_operator:
        b = operand.top();
        operand.pop();
        a = operand.top();
        operand.pop();
        operand.push(a / b);
        break;
    case Type::percent_operator:
        b = operand.top();
        operand.pop();
        a = operand.top();
        operand.pop();
        operand.push(a * (b / 100));
        break;
    };

    stack.pop();
}


