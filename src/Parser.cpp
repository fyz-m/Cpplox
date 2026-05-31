#include "Parser.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>
#include <vector>


Parser::Parser(const std::vector<Token>&& tokens)
        : tokens{std::move(tokens)} {}

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    
    // Left child node of the binary node
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL})) {
          Token operator_ = previous(); 
          auto right = comparison(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }
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