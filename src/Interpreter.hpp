#pragma once
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Token.hpp"
#include <memory>
#include <stdexcept>
#include <vector>
#include "Environment.hpp"

struct envGuard;

class Interpreter : public Visitor, public StmtVisitor {

    friend envGuard;  

    private:

        // Since visit methods return void, the result of an expression
        // evaluation is stored in this member
        literaltypes value;

        const Environment& globalEnvironment = Environment::getGlobal();
        Environment* environment = &Environment::getGlobal();

    public:
        
        // Interpreter API 
        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);

    private:

        // Statement node visitor implementation
        void visit(PrintStmt& stmt);
        void visit(ExpressionStmt& stmt);
        void visit(VarDeclarationStmt& stmt);
        void visit(BlockStmt& stmt);
        void visit(ifStmt& stmt);
        void visit(whileStmt& stmt);

        // Expression node visitor implementation
        void visit(Binary& expr); 
        void visit(Literal& expr);
        void visit(Grouping& expr);
        void visit(Unary& expr);
        void visit(Variable& expr);
        void visit(Assignment& expr); 
        void visit(Logical& expr); 

        // Execute a statement
        void execute(Stmt& statement);

        void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, Environment& newEnvironment);

        // Evaluate an expression and return the result
        literaltypes evaluate(Expr& expr);
 
        bool isTruthy(literaltypes& val);  

        bool isEqual(literaltypes& val1, literaltypes& val2);

        // Throws runtime error if operand is not a double 
        void checkifOperandisNumber(const Token& operator_, const literaltypes& operand);

        void checkifOperandsAreNumber(const Token& operator_, const literaltypes& left, const literaltypes& right);

        // Convert a Lox literal into string  
        std::string stringify(literaltypes& value);
};

class RuntimeError : public std::runtime_error {
    
    
    public:
    const Token& token;
    RuntimeError(const Token& token, const std::string& Message)
                : std::runtime_error(Message), token{token} {}

};


// This ensures that the enviroment is restored 
// when an exception is thrown  
struct envGuard {
       Interpreter& interpreter;
       Environment* previousEnv;

       envGuard(Interpreter& interpreter, Environment* previousEnv)
               : interpreter{interpreter}, previousEnv{previousEnv} {}

       ~envGuard() {
            interpreter.environment = previousEnv;
       }

};
