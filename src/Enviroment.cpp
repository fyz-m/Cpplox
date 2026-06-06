#include "Enviroment.hpp"
#include "Interpreter.hpp"
#include "Token.hpp"
#include <stdexcept>
#include <string>

void Enviroment::define(const std::string& name, const literaltypes& value) {
    Map[name] = std::move(value);
}

literaltypes Enviroment::get(const Token& name) {
    
    try {
        return Map.at(name.lexeme);

    } catch (const std::out_of_range& e) {
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
}
