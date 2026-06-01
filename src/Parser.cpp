#include "Parser.hpp"
#include "Expr.hpp"
#include "Lox.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>
#include <variant>
#include <vector>


Parser::Parser(const std::vector<Token>&& tokens)
       : tokens{std::move(tokens)} 
       {}

std::unique_ptr<Expr> Parser::parse() {

    try { 
        return expression(); 
    }
    catch (const ParseError& e) {
        return nullptr;
    }
}   

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    
    // Left child node of the binary AST node
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL})) {
          Token operator_ = std::move(previous()); 
          auto right = comparison(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {

    // Left child node of the binary AST node
    auto expr = term();

    while (match({
                             TokenType::GREATER,
                             TokenType::GREATER_EQUAL,
                             TokenType::LESS,
                             TokenType::LESS_EQUAL
                                 })) {

          Token operator_ = std::move(previous()); 
          auto right = term(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::term() {

    // Left child node of the binary AST node
    auto expr = factor();

    while (match({
                             TokenType::PLUS,
                             TokenType::MINUS
                                 })) {

          Token operator_ = std::move(previous()); 
          auto right = factor(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }
    
    return expr;

}

std::unique_ptr<Expr> Parser::factor() {

    // Left child node of the binary AST node
    auto expr = unary();

    while (match({
                             TokenType::STAR,
                             TokenType::SLASH
                                 })) {

          Token operator_ = std::move(previous()); 
          auto right = term(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {

    if (match({TokenType::BANG, TokenType::MINUS})) {
        auto operator_ = std::move(previous());
        auto expr = unary();
        return std::make_unique<Unary>(std::move(expr), std::move(operator_));
    }
    
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {

    if (match({TokenType::TRUE})) 
        return std::make_unique<Literal>(true);

    if (match({TokenType::FALSE})) 
        return std::make_unique<Literal>(false);

    if (match({TokenType::NIL})) 
        return std::make_unique<Literal>(std::monostate{});

    if (match({TokenType::STRING, TokenType::NUMBER})) 
        return std::make_unique<Literal>(std::move(previous().literal));

    if (match({TokenType::LEFT_PAREN})) {     
        
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");  
        return std::make_unique<Grouping>(std::move(expr));
    }
    
    throw error(peek(), "Expect expression.");
}


bool Parser::match(std::initializer_list<TokenType> expected_types) {

  for (auto expected_type : expected_types) {
    
      if (check(expected_type)) {
         advance();
         return true;
      }
  }
    return false;
}

bool Parser::check(TokenType expected_type) {
    return peek().type == expected_type;
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::_EOF;
}

Token& Parser::peek() {
    return tokens[current];
}
 
Token& Parser::previous() {
    return tokens[current-1];
}

Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}  

Token& Parser::consume(TokenType type, std::string_view error_message) {
    if (check(type)) return advance();
    throw error(peek(), error_message);
}

ParseError Parser::error(Token& token, std::string_view error_message) {
    Lox::error(token, error_message);
    return ParseError();
}

void Parser::synchronize() {
    advance();

    while(!isAtEnd()) {
        if(previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
            return;
            default: break;
        }
        advance();
    }
}

