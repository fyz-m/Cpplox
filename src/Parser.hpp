#pragma once
#include "Token.hpp"
#include <vector>

class Parser {

    private:

        const std::vector<Token> tokens;
        int current {0};
    
    public:

        Parser(const std::vector<Token>&& tokens);
    
    private:
        
};
