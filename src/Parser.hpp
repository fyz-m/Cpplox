#pragma once
#include "Expr.hpp"
#include "Token.hpp"
#include <initializer_list>
#include <memory>
#include <vector>

class Parser {

    private:

        std::vector<Token> tokens;

        // Token pointer/index 
        int current {0};
    
    public:

        Parser(const std::vector<Token>&& tokens);
    
    private:
    
        std::unique_ptr<Expr> expression();

        std::unique_ptr<Expr> equality();

        std::unique_ptr<Expr> comparison();

        std::unique_ptr<Expr> term();

        std::unique_ptr<Expr> factor();

        std::unique_ptr<Expr> unary();

        std::unique_ptr<Expr> primary();

        // Returns true if current token is equal to arg type
        bool check(TokenType expected_type);

        // Conditional advance        
        bool match(std::initializer_list<TokenType> expected_types);

        bool isAtEnd();

        // Return current token but doesn't consume it
        Token& peek();

        // Return previously consumed token
        Token& previous();

        // Consume token at index current
        Token& advance();

};
