#pragma once
#include "Expr.hpp"
#include "Token.hpp"
#include <memory>
#include <vector>

// Forward declarations
struct PrintStmt;
struct ExpressionStmt;
struct VarDeclarationStmt;
struct BlockStmt;

// Interface for functions to act on a statement node
struct StmtVisitor {
    virtual void visit(PrintStmt& stmt) = 0;
    virtual void visit(ExpressionStmt& stmt) = 0;
    virtual void visit(VarDeclarationStmt& stmt) = 0;
    virtual void visit(BlockStmt& stmt) = 0;
};

struct Stmt {

    Stmt() = default;

    virtual ~Stmt() = default;

    virtual void accept(StmtVisitor& v) = 0;
};

struct PrintStmt : public Stmt {

    std::unique_ptr<Expr> expression;

    PrintStmt(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};

struct ExpressionStmt : public Stmt {

    std::unique_ptr<Expr> expression;

    ExpressionStmt(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};

struct VarDeclarationStmt : public Stmt {

    std::unique_ptr<Expr> initializer;
    const Token var_name;
    
    VarDeclarationStmt(std::unique_ptr<Expr> initializer, Token&& var_name) 
                      : initializer{std::move(initializer)},
                        var_name{std::move(var_name)}
                      {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};

struct BlockStmt : public Stmt {

    const std::vector<std::unique_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
             : statements{std::move(statements)} {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};