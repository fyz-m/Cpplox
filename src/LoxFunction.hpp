#pragma once
#include "Interpreter.hpp"
#include "Stmt.hpp"
#include "Value.hpp"
#include "LoxCallable.hpp"
#include <cstddef>
#include <variant>

class LoxFunction : public LoxCallable {

    private:

        const functionStmt& declaration;

    public:

        LoxFunction(functionStmt& declaration)
                    : declaration{declaration} {} 

        LoxLiteral call(Interpreter& interpreter, std::vector<LoxLiteral>& arguments) override {

            // Create a new enviroment every time the function is called
            auto functionEnv = std::make_unique<Environment>(&Environment::getGlobal());
            
            // Initialize the function enviroment:
            // Add parameters as variables whose values are provided
            // by the given arguments to the function call 
            
            for (int i = 0; i < declaration.parameters.size(); i++) {
                auto parameterName = declaration.parameters[i].lexeme;
                auto parameterValue = arguments[i];
                functionEnv->define(parameterName, std::move(parameterValue));
            }
           try {
                interpreter.executeBlock(declaration.bodyStatements, *functionEnv); 
           } catch (const ReturnException& r) {
                return r.returnValue;
           }
           // Return nil
           return std::monostate{};
        }

        size_t arity() override {
            return declaration.parameters.size();
        }

};