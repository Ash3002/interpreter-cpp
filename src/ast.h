#ifndef AST_H
#define AST_H

#include <memory>
#include <string>

// Base class for expressions.
struct Expr {
    virtual ~Expr() = default;
};

using ExprPtr = std::unique_ptr<Expr>;

// Expression for numeric literals.
struct LiteralExpr : public Expr {
    std::string value;
    LiteralExpr(const std::string& value) : value(value) {}
};

// Expression for binary operations.
struct BinaryExpr : public Expr {
    ExprPtr left;
    std::string op; // For example, "+"
    ExprPtr right;
    
    BinaryExpr(ExprPtr left, const std::string& op, ExprPtr right)
      : left(std::move(left)), op(op), right(std::move(right)) {}
};

#endif // AST_H
