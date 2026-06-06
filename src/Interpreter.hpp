#pragma once
#include "Expr.hpp"
#include "Parser.hpp"
#include "Token.hpp"
#include <stdexcept>

class Interpreter : public Visitor, public StmtVisitor {
    
    private:

        literaltypes value;

    public:

        // Interpreted API 
        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);

        // Statement node visitor implementation
        void visit(PrintStmt& stmt);
        void visit(ExpressionStmt& stmt);

        // Expression node visitor implementation
        void visit(Binary& expr); 
        void visit(Literal& expr);
        void visit(Grouping& expr);
        void visit(Unary& expr);

    private: 

        // Execute a statement
        void execute(Stmt& statement);

        // Evaluate an expression and return the result
        literaltypes evaluate(Expr& expr);
 
        bool isTruthy(literaltypes& val);  

        bool isEqual(literaltypes& val1, literaltypes& val2);

        // Throws runtime error if operand is not a double 
        void checkifOperandisNumber(const Token& operator_, const literaltypes& operand);

        void checkifOperandsAreNumber(const Token& operator_, const literaltypes& left, const literaltypes& right);

        // Convert a literal into string  
        std::string stringify(literaltypes& value);
};

class RuntimeError : public std::runtime_error {
    
    
    public:
    const Token& token;
    RuntimeError(const Token& token, const std::string& Message)
                : std::runtime_error(Message), token{token} {}

};