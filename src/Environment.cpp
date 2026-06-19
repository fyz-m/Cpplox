#include "Environment.hpp"
#include "Interpreter.hpp"
#include "Token.hpp"
#include <string>


Environment& Environment::getGlobal() {
    static Environment globalInstance;
    return globalInstance;
}
void Environment::define(const std::string& name, LoxLiteral&& value) {
    Map[name] = std::move(value);
}   

void Environment::assign(const Token& name, LoxLiteral&& value) {

    if (Map.contains(name.lexeme)) {
        Map[name.lexeme] = std::move(value); return;
    }

    // Recursively check parent enviroments for variable
    if (enclosingEnv != nullptr) {
        enclosingEnv->assign(name, std::move(value));
        return;
    }

    throw RuntimeError(name, "Assignment to undefined variable '" + name.lexeme + "'.");
}

LoxLiteral Environment::get(const Token& name) const {
    
    if (Map.contains(name.lexeme)) return Map.at(name.lexeme);
    
    // Recursively check parent enviroments for variable
    if (enclosingEnv != nullptr) {
        return enclosingEnv->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    
}
