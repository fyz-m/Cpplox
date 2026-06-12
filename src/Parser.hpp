#pragma once
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Token.hpp"
#include <exception>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>


class ParseError : public std::exception {};


class Parser {

    private:

        std::vector<Token> tokens;

        // Token pointer/index 
        int current {0};
    
    public:

        Parser(std::vector<Token>&& tokens);

        std::vector<std::unique_ptr<Stmt>> parse();

    private:

        // These functions return an AST for a given statement
        //
        std::unique_ptr<Stmt> declaration();

        std::unique_ptr<VarDeclarationStmt> varDeclaration();

        std::unique_ptr<Stmt> statement();

        std::unique_ptr<ExpressionStmt> expressionStatement();

        std::unique_ptr<PrintStmt> printStatement();
        
        std::unique_ptr<ifStmt> ifStatement();

        std::unique_ptr<whileStmt> whileStatement();

        // These functions return an AST for a given expression
        // Ordered from lowest -> highest precedence  
        std::unique_ptr<Expr> expression();
                
        std::unique_ptr<Expr> assignment();

        std::unique_ptr<Expr> logicalOr();

        std::unique_ptr<Expr> logicalAnd();

        std::unique_ptr<Expr> equality();

        std::unique_ptr<Expr> comparison();

        std::unique_ptr<Expr> term();

        std::unique_ptr<Expr> factor();

        std::unique_ptr<Expr> unary();

        std::unique_ptr<Expr> primary();

        // Return a vector of statements enlosed in curly braces
        std::vector<std::unique_ptr<Stmt>> block();

        // Returns true if current token is equal to arg type
        bool check(TokenType expected_type);

        // Conditional advance        
        bool match(std::initializer_list<TokenType> expected_types);

        // Returns true if final token reached
        bool isAtEnd();

        // Return current token but doesn't consume it
        Token& peek();

        // Return previously consumed token
        Token& previous();

        // Consume token at index current
        Token& advance();

        // Consume current token which must be of the type given in argument
        Token& consume(TokenType type, std::string_view error_message);

        ParseError error(Token& token, std::string_view error_message);

        // Discard tokens until statement is found
        void synchronize();

};
