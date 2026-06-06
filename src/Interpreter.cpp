#include "Interpreter.hpp"
#include "Lox.hpp"
#include "Parser.hpp"
#include "Token.hpp"
#include <cerrno>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>


void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(*statement);
        } 
    } catch (const RuntimeError& e) {
        Lox::runtimeError(e);
    }
}

void Interpreter::visit(PrintStmt& stmt) {
     auto value = evaluate(*stmt.expression);
     std::cout << stringify(value);     
}

void Interpreter::visit(ExpressionStmt& stmt) {
    evaluate(*stmt.expression);
}
void Interpreter::visit(Binary &expr) {

    auto lhs = evaluate(*expr.left.get());
    auto rhs = evaluate(*expr.right.get());

    switch (expr.operator_.type) {

        case TokenType::MINUS:
            checkifOperandsAreNumber(expr.operator_, lhs, rhs);
            value = std::get<double>(lhs) - std::get<double>(rhs);
            break;

        case TokenType::SLASH:
            checkifOperandsAreNumber(expr.operator_, lhs, rhs);
            value = std::get<double>(lhs) / std::get<double>(rhs);
            break;
        
        case TokenType::STAR:
            checkifOperandsAreNumber(expr.operator_, lhs, rhs);
            value = std::get<double>(lhs) * std::get<double>(rhs);
            break;

        case TokenType::PLUS: 
            // Adding numeric literals 
            if (std::holds_alternative<double>(lhs) &&
                std::holds_alternative<double>(rhs)) { 
                value = std::get<double>(lhs) + std::get<double>(rhs);
                break;
            }
            // Adding string literals
            if (std::holds_alternative<std::string>(lhs) &&
                std::holds_alternative<std::string>(rhs)) { 
                value = std::get<std::string>(lhs) + std::get<std::string>(rhs);
                break;
            }
            throw RuntimeError(expr.operator_, "Cannot add string and numeric literal");    
            break;
        
        case TokenType::GREATER:
            checkifOperandsAreNumber(expr.operator_, lhs, rhs);
            value = std::get<double>(lhs) > std::get<double>(rhs);
            break;

        case TokenType::GREATER_EQUAL:
            checkifOperandsAreNumber(expr.operator_, lhs, rhs);
            value = std::get<double>(lhs) >= std::get<double>(rhs);
            break;
    
        case TokenType::LESS:
            checkifOperandsAreNumber(expr.operator_, lhs, rhs);
            value = std::get<double>(lhs) < std::get<double>(rhs);
            break;
            
        case TokenType::LESS_EQUAL:
            checkifOperandsAreNumber(expr.operator_, lhs, rhs);
            value = std::get<double>(lhs) <= std::get<double>(rhs);
            break;

        case TokenType::EQUAL_EQUAL:
            value = isEqual(lhs, rhs);
            break;
        
        case TokenType::BANG_EQUAL:
            value = !isEqual(lhs, rhs);
            break;

        // error 
        default: break;
    }
}

void Interpreter::visit(Literal& expr) {
    // We want to extract the value out of variant
    value = expr.value;
}

void Interpreter::visit(Grouping& expr) {
    evaluate(expr);
}

void Interpreter::visit(Unary& expr) {

    // Check if value is actually a double
    
    auto rhs = evaluate(*expr.expression.get());

    switch (expr.operator_.type) {
        
        case TokenType::MINUS: {
          checkifOperandisNumber(expr.operator_, rhs);
          value = -std::get<double>(rhs);
          break; }

        case TokenType::BANG:
          value = isTruthy(rhs);

        default: break;
        // error
    }
}

literaltypes Interpreter::evaluate(Expr& expr) {
     expr.accept(*this);
     return value;
}

void Interpreter::execute(Stmt& statement) {
    statement.accept(*this);
} 

bool Interpreter::isTruthy(literaltypes& val) {
    // if value is null
    if (val.index() == 0) 
        return false;
    
    if (auto boolean = std::get_if<bool>(&val))
        return *boolean;

    // All other values 
    return true;
    
}

bool Interpreter::isEqual(literaltypes& val1, literaltypes& val2) {
    return val1 == val2;
}

void Interpreter::checkifOperandisNumber(const Token& operator_, const literaltypes& operand) {

    if (std::holds_alternative<double>(operand)) return;

    throw RuntimeError(operator_, "Operand must be a number.");
}

void Interpreter::checkifOperandsAreNumber(const Token& operator_, const literaltypes& left, const literaltypes& right) {

    if (std::holds_alternative<double>(left) && 
        std::holds_alternative<double>(right)) return;

    throw RuntimeError(operator_, "Operand must be a number.");
}

std::string Interpreter::stringify(literaltypes& value) {

    if (std::holds_alternative<std::monostate>(value)) return "nil";
    
    if (auto number =  std::get_if<double>(&value)) {
         return std::to_string(*number);   
    };

    if (auto boolean =  std::get_if<bool>(&value)) {
       return *boolean ? "true" : "false"; 
    }; 

    return std::get<std::string>(value);
}