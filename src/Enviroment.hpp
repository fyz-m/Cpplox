#pragma  once
#include "Token.hpp"
#include <unordered_map>
// We can std::move into map instead of copying
class Enviroment {

    private:

        std::unordered_map<std::string , literaltypes> Map;
    
    public:

        // Define a new variable, allows reassignment
        void define(const std::string& name, literaltypes&& value);

        // Assign an exisiting variable a new value
        // RuntimeError reported if variable doesn't exist
        void assign(const Token& name, literaltypes&& value);

        // Returns the value of a variable 
        // RuntimeError is reported if argument is an undefined variable  
        literaltypes get(const Token& name);
};

