#pragma once
#include "TokenType.hpp"
#include <string>

class Token {

  public:

    const TokenType type;

    const std::string lexeme;

    const std::string literal;

    const int line;

    Token(
          TokenType type, 
          std::string lexeme, 
          std::string literal,
          int line
         );

    std::string toString();
    
};