#pragma  once
#include "Token.hpp"
#include <unordered_map>

class Environment {
 
    private:

        std::unordered_map<std::string, LoxLiteral> Map;
        
        Environment* enclosingEnv;
    
    public:

        Environment(const Environment&) = delete;

        Environment(Environment* enclosing) : enclosingEnv(enclosing) {}

        // Return reference to the global enviroment 
        static Environment& getGlobal();

        // Define a new variable, allows reassignment
        void define(const std::string& name, LoxLiteral&& value);

        // Assign an exisiting variable a new value
        // RuntimeError reported if variable doesn't exist
        void assign(const Token& name, LoxLiteral&& value);

        // Returns the value of a variable 
        // RuntimeError is reported if argument is an undefined variable  
        LoxLiteral get(const Token& name) const;

    private:

        Environment() {};
};

