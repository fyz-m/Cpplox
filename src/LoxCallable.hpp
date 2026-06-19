#pragma once
#include "Interpreter.hpp"
#include "Value.hpp"
#include <cstddef>

class Interpreter;

struct LoxCallable {
    virtual ~LoxCallable() = default;
    virtual size_t arity() = 0;
    virtual LoxLiteral 
        call(Interpreter& interpreter, std::vector<LoxLiteral>& arguments) = 0;
};
