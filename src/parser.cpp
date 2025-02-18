#include "parser.h"
#include <iostream>
#include <cstdlib>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), current(0) {}

std::unique_ptr<Expr> Parser::parse() {
    return expression();
}

std::unique_ptr<Expr> Parser::expression() {
    // For now, we only support addition.
    return addition();
}

std::unique_ptr<Expr> Parser::addition() {
    auto expr = primary();
    while (!isAtEnd() && peek().type == TOKEN_PLUS) {
        Token op = advance();
        auto right = primary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op.lexeme, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (!isAtEnd() && peek().type == TOKEN_NUMBER) {
        Token numberToken = advance();
        return std::make_unique<LiteralExpr>(numberToken.literal);
    }
    std::cerr << "Parser error: Expected number." << std::endl;
    std::exit(1);
}

Token Parser::advance() {
    return tokens[current++];
}

bool Parser::isAtEnd() {
    return current >= tokens.size() || tokens[current].type == TOKEN_EOF;
}

Token Parser::peek() {
    return tokens[current];
}
