#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <variant>
#include <stack>

enum class ID {

    ID_EDIT_BOX = 101,
    ID_BUTTON = 102
};

struct VerticalLayout {
    int currentY;
    int padding;
    int marginX;
    int height;

    VerticalLayout(int startY, int spacing, int startX)
        : currentY{ startY }, padding{ spacing }, marginX{ startX }, height{ 30 } {
    }

    int Append(int elementHeight) {
        int allocatedY = currentY;
        currentY += elementHeight + padding;
        return allocatedY;
    }
};

enum class Type {

    operand,
    plus_operator,
    sub_operator,
    div_operator,
    mul_operator,
    percent_operator,
    unary_sub
};

enum class Precedence {

    PLUS = 10,
    SUB = 10,
    MUL = 11,
    DIV = 11,
    PER = 11,
    UNARY = 12

};

enum class ParserState {
    EXPECT_OPERAND,
    EXPECT_OPERATOR
};

struct Token {

    Token(Type t, Precedence p)
        : type(t), data(static_cast<int>(p)) {
    }

    Token(Type t, double p)
        : type{ t }, data{ p } {
    }

    Token() : type{ Type::operand } {
    }

    Type type;
    std::variant<int, double> data;
};

class Parser {

    std::vector<Token> tokens;
    std::stack<Token> stack; // * *
    std::stack<double> operand; // 3 5  
    std::wstring_view source;
    HWND edit;
    int index;
public:
    Parser(std::wstring_view s, HWND hEdit) : source{ s }, edit{ hEdit }, index{ 0 } {

        lexer();
    }

    int peek() {
        if (index < source.size())
            return static_cast<char>(source[index]);
        else
            return '\0';
    }

    void advance() {

        index++;

    }

    void lexer();
    void parser();
    void eval();
};
