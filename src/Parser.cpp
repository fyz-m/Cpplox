#include "Parser.hpp"
#include "Token.hpp"
#include <utility>
#include <vector>


Parser::Parser(const std::vector<Token>&& tokens)
        : tokens{std::move(tokens)} {}

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    
    auto expr = comparison();

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {

}

std::unique_ptr<Expr> Parser::term() {

}

std::unique_ptr<Expr> Parser::factor() {

}

std::unique_ptr<Expr> Parser::unary() {

}

std::unique_ptr<Expr> Parser::primary() {

}


bool match(std::initializer_list<TokenType> types) {

}

bool Parser::isAtEnd() {
    if (peek().type == TokenType::_EOF) return false;
    return true;
}

Token& Parser::peek() {
    return tokens[current];
}

Token& Parser::previous() {
    return tokens[current-1];
}

Token& Parser::advance() {
    return tokens[current++];
}