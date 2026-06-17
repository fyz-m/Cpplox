#pragma once
#include "Token.hpp"
#include <memory>
#include <utility>
#include <vector>


// Forward declarations
struct Binary;
struct Literal;
struct Grouping;
struct Unary;
struct Variable;
struct Assignment;
struct Logical;
struct Call;

struct Visitor {

    virtual void visit(Binary& expr) = 0; 
    virtual void visit(Literal& expr) = 0;
    virtual void visit(Grouping& expr) = 0;
    virtual void visit(Unary& expr) = 0;
    virtual void visit(Variable& expr) = 0;
    virtual void visit(Assignment& expr) = 0;
    virtual void visit(Logical& expr) = 0;
    virtual void visit(Call& expr) = 0;

};

struct Expr {

    Expr() = default;

    virtual ~Expr() = default;

    virtual void accept(Visitor& v) = 0;
};

struct Binary : public Expr {

     std::unique_ptr<Expr> left;
     const Token           operator_;
     std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, 
           Token&& operator_,
           std::unique_ptr<Expr> right)
           : left{std::move(left)}, 
             operator_{std::move(operator_)}, 
             right{std::move(right)}
           {}
    
     void accept(Visitor& v) override {
        v.visit(*this);
     }
};

struct Literal : public Expr {

    const literaltypes value;

    Literal(literaltypes&& value) : value{std::move(value)} {}

    void accept(Visitor& v) override {
        v.visit(*this);
     }

};

struct Grouping : public Expr {

    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

    void accept(Visitor& v) override {
        v.visit(*this);
     }

};

struct Unary : public Expr {

    std::unique_ptr<Expr> expression;
    const Token           operator_;

    Unary(std::unique_ptr<Expr> expression,
          Token&& operator_) 
          : expression{std::move(expression)},
            operator_{std::move(operator_)}
          {}

    void accept(Visitor& v) override {
        v.visit(*this);
     }

};

struct Variable : public Expr {

    Token name;

    Variable(Token&& name) : name{std::move(name)} {}

    void accept(Visitor& v) override {
        v.visit(*this);
     }
};

struct Assignment : public Expr {

    const Token name; // name of variable being assigned to
    std::unique_ptr<Expr> value;

    Assignment(Token&& name, std::unique_ptr<Expr> value) 
              : name{std::move(name)}, 
                value{std::move(value)} {} 

    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

struct Logical : public Expr {

     std::unique_ptr<Expr> left;
     const Token           operator_;
     std::unique_ptr<Expr> right;

    Logical(std::unique_ptr<Expr> left, 
           Token&&                operator_,
           std::unique_ptr<Expr>  right)
           : left{std::move(left)}, 
             operator_{std::move(operator_)}, 
             right{std::move(right)}
           {}
    
     void accept(Visitor& v) override {
        v.visit(*this);
     }
};

struct Call : public Expr {

     std::unique_ptr<Expr>              callee;
     const Token                        closing_paren;
     std::vector<std::unique_ptr<Expr>> arguments;  

    Call(std::unique_ptr<Expr>              callee, 
         Token&&                            closing_paren,
         std::vector<std::unique_ptr<Expr>> arguments)

         : callee{std::move(callee)}, 
           closing_paren{std::move(closing_paren)}, 
           arguments{std::move(arguments)}
        {}
    
     void accept(Visitor& v) override {
        v.visit(*this);
     }
};