#include "Token.hpp"
#include <memory>


// Forward declarations
struct Binary;
struct Literal;
struct Grouping;
struct Unary;

struct Visitor {

    virtual void visit(Binary& expr) = 0; 
    virtual void visit(Literal& expr) = 0;
    virtual void visit(Grouping& expr) = 0;
    virtual void visit(Unary& expr) = 0;

};

struct Expr {

    Expr() = default;

    virtual ~Expr() = default;

    virtual void accept(Visitor& v) = 0;
};

struct Binary : public Expr {

    const std::unique_ptr<const Expr> left;
    const Token                       operator_;
    const std::unique_ptr<const Expr> right;

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

    const std::unique_ptr<const Expr> expression;

    Grouping(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

    void accept(Visitor& v) override {
        v.visit(*this);
     }

};

struct Unary : public Expr {

    const std::unique_ptr<const Expr> expression;
    const Token                       operator_;

    Unary(std::unique_ptr<Expr> expression,
          Token&& operator_) 
          : expression{std::move(expression)},
            operator_{std::move(operator_)}
          {}

    void accept(Visitor& v) override {
        v.visit(*this);
     }

};
