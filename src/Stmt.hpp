#include "Expr.hpp"
#include <memory>

// Forward declarations
struct PrintStmt;
struct ExpressionStmt;

// Interface for functions to act on a statement node
struct StmtVisitor {
    void visit(PrintStmt& stmt);
    void visit(ExpressionStmt& stmt);
};

struct Stmt {

    Stmt() = default;

    virtual ~Stmt() = default;

    virtual void accept(StmtVisitor& v) = 0;
};

struct PrintStmt : public Stmt {

    std::unique_ptr<Expr> expression;

    PrintStmt(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

    void accept(StmtVisitor &v) override {
        v.visit(*this);
    }
};

struct ExpressionStmt : public Stmt {

    std::unique_ptr<Expr> expression;

    ExpressionStmt(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

    void accept(StmtVisitor &v) override {
        v.visit(*this);
    }
};
