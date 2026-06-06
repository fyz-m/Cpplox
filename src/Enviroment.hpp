#pragma  once
#include "Token.hpp"
#include <unordered_map>
// We can std::move into map instead of copying
class Enviroment {

    private:

        std::unordered_map<std::string , literaltypes> Map;
    
    public:

        void define(const std::string& name, const literaltypes& value);

        literaltypes get(const Token& name);
};

