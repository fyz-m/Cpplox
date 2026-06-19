#pragma once
#include <memory>
#include <string>
#include <variant>

struct LoxCallable;

// This is the runtime representation of literal values in the Lox language
// Because Lox is interpreted, we type-check at runtime 
using LoxLiteral = std::variant<
    std::monostate,                 // Represents Lox's "nil" 
    std::string,
    double, 
    bool,
    std::shared_ptr<LoxCallable>>;  // Shared ptr so variant can be copied.
                                    // Because unique_ptr has a deleted constructor,
                                    // we will not be able to copy a LoxLiteral