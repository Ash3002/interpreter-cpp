#include "ast_printer.h"
#include "ast.h"
#include <sstream>

std::string AstPrinter::print(Expr* expr) {
    if (auto lit = dynamic_cast<LiteralExpr*>(expr)) {
        return lit->value;
    } else if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        return parenthesize(bin->op, {bin->left.get(), bin->right.get()});
    }
    return "";
}

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<Expr*>& exprs) {
    std::ostringstream oss;
    oss << "(" << name;
    for (auto expr : exprs) {
        oss << " " << print(expr);
    }
    oss << ")";
    return oss.str();
}
