#include "Enviroment.hpp"
#include "Interpreter.hpp"
#include "Token.hpp"
#include <stdexcept>
#include <string>

void Enviroment::define(const std::string& name, literaltypes&& value) {
    Map[name] = std::move(value);
}   

void Enviroment::assign(const Token& name, literaltypes&& value) {
    if (Map.contains(name.lexeme)) {
        Map[name.lexeme] = std::move(value); return;
    }
    throw RuntimeError(name, "Assignment to undefined variable '" + name.lexeme + "'.");
}

literaltypes Enviroment::get(const Token& name) {
    
    try {
        return Map.at(name.lexeme);

    } catch (const std::out_of_range& e) {
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
}
