#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    // Returns the root of the AST.
    std::unique_ptr<Expr> parse();
    
private:
    const std::vector<Token>& tokens;
    int current;
    
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> addition();
    std::unique_ptr<Expr> primary();
    
    Token advance();
    bool isAtEnd();
    Token peek();
};

#endif // PARSER_H
