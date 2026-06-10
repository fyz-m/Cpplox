#pragma  once
#include "Token.hpp"
#include <unordered_map>

class Environment {

    private:

        std::unordered_map<std::string , literaltypes> Map;
        
        Environment* enclosingEnv {nullptr};
    
    public:

        Environment(const Environment&) = delete;

        Environment(Environment& enclosing) : enclosingEnv(&enclosing) {}

        // Ensure only one global environment exists
        static Environment& getGlobal();

        // Define a new variable, allows reassignment
        void define(const std::string& name, literaltypes&& value);

        // Assign an exisiting variable a new value
        // RuntimeError reported if variable doesn't exist
        void assign(const Token& name, literaltypes&& value);

        // Returns the value of a variable 
        // RuntimeError is reported if argument is an undefined variable  
        literaltypes get(const Token& name) const;

    private:

        Environment() {};
};

