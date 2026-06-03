#pragma once
#include "Expr.hpp"
#include "Token.hpp"

class Interpreter : public Visitor {
    
    private:

        literaltypes value;

    public:
    
        void visit(Binary& expr); 
        void visit(Literal& expr);
        void visit(Grouping& expr);
        void visit(Unary& expr);

    private: 

        // evaluate an expression and return the result
        literaltypes evaluate(Expr& expr);

        // 
        bool isTruthy(literaltypes& val);  
};