#include "Interpreter.hpp"
#include "Token.hpp"
#include <cerrno>
#include <variant>



void Interpreter::visit(Binary &expr) {

    auto lhs = evaluate(*expr.left.get());
    auto rhs = evaluate(*expr.right.get());

    switch (expr.operator_.type) {

        case TokenType::MINUS:
            value = std::get<double>(lhs) - std::get<double>(rhs);
            break;

        case TokenType::SLASH:
            value = std::get<double>(lhs) / std::get<double>(rhs);
            break;
        
        case TokenType::STAR:
            value = std::get<double>(lhs) * std::get<double>(rhs);
            break;

        case TokenType::PLUS: {
            // Adding numeric literals 
            if (std::holds_alternative<double>(lhs) &&
                std::holds_alternative<double>(rhs)) { 
                value = std::get<double>(lhs) + std::get<double>(rhs);
            }
            // Adding string literals
            if (std::holds_alternative<std::string>(lhs) &&
                std::holds_alternative<std::string>(rhs)) { 
                value = std::get<std::string>(lhs) + std::get<std::string>(rhs);
            }
            break;
        }
        
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
        
        case TokenType::MINUS:
          value = -std::get<double>(rhs);
          break;

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

bool Interpreter::isTruthy(literaltypes& val) {
    // if value is null
    if (val.index() == 0) 
        return false;
    
    if (auto boolean = std::get_if<bool>(&val))
        return *boolean;

    // All other values 
    return true;
    
}