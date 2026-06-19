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
struct ifStmt;
struct whileStmt;
struct breakStmt;
struct functionStmt;
struct returnStmt;

// Interface for functions to act on a statement node
struct StmtVisitor {
    virtual void visit(PrintStmt& stmt) = 0;
    virtual void visit(ExpressionStmt& stmt) = 0;
    virtual void visit(VarDeclarationStmt& stmt) = 0;
    virtual void visit(BlockStmt& stmt) = 0;
    virtual void visit(ifStmt& stmt) = 0;
    virtual void visit(whileStmt& stmt) = 0;
    virtual void visit(breakStmt& stmt) = 0;
    virtual void visit(functionStmt& stmt) = 0;
    virtual void visit(returnStmt& stmt) = 0;
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

    BlockStmt(std::vector<std::unique_ptr<Stmt>>&& statements)
             : statements{std::move(statements)} {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};

struct ifStmt : public Stmt {

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> trueBranch;  
    std::unique_ptr<Stmt> falseBranch;

    ifStmt(std::unique_ptr<Expr> condition,
           std::unique_ptr<Stmt> trueBranch,  
           std::unique_ptr<Stmt> falseBranch)
           : condition{std::move(condition)},
             trueBranch{std::move(trueBranch)},
             falseBranch{std::move(falseBranch)}  {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};

struct whileStmt : public Stmt {

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> bodyStatements;  
    
    whileStmt(std::unique_ptr<Expr> condition,
           std::unique_ptr<Stmt> bodyStatements)
           : condition{std::move(condition)},
             bodyStatements{std::move(bodyStatements)} 
             {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};

struct breakStmt : public Stmt {

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }

};

struct functionStmt : public Stmt {

    const Token name;
    std::vector<Token> parameters;
    std::vector<std::unique_ptr<Stmt>> bodyStatements;

    functionStmt(Token&& name,
                 std::vector<Token>&& parameters,
                 std::vector<std::unique_ptr<Stmt>>&& bodyStatements) 
                 : name{std::move(name)},
                   parameters{std::move(parameters)},  
                   bodyStatements{std::move(bodyStatements)}
                {}

     void accept(StmtVisitor &v) override {
        v.visit(*this);
    }
};

struct returnStmt : public Stmt {

    std::unique_ptr<Expr> retValue;

    returnStmt(std::unique_ptr<Expr> retValue) 
               : retValue{std::move(retValue)} {}

    void accept(StmtVisitor& v) override {
        v.visit(*this);
    }
};