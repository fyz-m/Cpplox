#pragma once
#include "Expr.hpp"
#include "Token.hpp"
#include <initializer_list>
#include <memory>
#include <vector>

class Parser {

    private:

        std::vector<Token> tokens;
        int current {0};
    
    public:

        Parser(const std::vector<Token>& tokens);
    
    private:
    
        std::unique_ptr<Expr> expression();

        std::unique_ptr<Expr> equality();

        std::unique_ptr<Expr> comparison();

        std::unique_ptr<Expr> term();

        std::unique_ptr<Expr> factor();

        std::unique_ptr<Expr> unary();

        std::unique_ptr<Expr> primary();

        bool match(std::initializer_list<TokenType> types);

        bool isAtEnd();

        Token& peek();

        Token& previous();

        Token& advance();


};
