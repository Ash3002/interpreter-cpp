#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "ast.h"
#include <string>
#include <vector>

class AstPrinter {
public:
    std::string print(Expr* expr);
private:
    std::string parenthesize(const std::string& name, const std::vector<Expr*>& exprs);
};

#endif // AST_PRINTER_H
