#pragma once
#include "AstPrinter.hpp"
#include "Expr.hpp"
#include "Token.hpp"
#include <stdexcept>

class Interpreter : public Visitor {
    
    private:

        literaltypes value;

    public:

        void Interpret(Expr& expression);

        void visit(Binary& expr); 
        void visit(Literal& expr);
        void visit(Grouping& expr);
        void visit(Unary& expr);

    private: 

        // evaluate an expression and return the result
        literaltypes evaluate(Expr& expr);

        // 
        bool isTruthy(literaltypes& val);  

        bool isEqual(literaltypes& val1, literaltypes& val2);

        // Throws runtime error if operand is not a double 
        void checkifOperandisNumber(const Token& operator_, const literaltypes& operand);

        void checkifOperandsAreNumber(const Token& operator_, const literaltypes& left, const literaltypes& right);

        std::string stringify(literaltypes& value);
};

class RuntimeError : public std::runtime_error {
    
    const Token& token;
    public:
    
    RuntimeError(const Token& token, const std::string& Message)
                : std::runtime_error(Message), token{token} {}

};