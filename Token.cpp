#include "Token.hpp"
#include <sstream>


  Token::Token(
          TokenType type, 
          std::string lexeme, 
          std::string literal,
          int line
          )
          : type{type}, 
            lexeme{std::move(lexeme)}, 
            literal{std::move(literal)}, 
            line{line}
            {
            }
          
    std::string Token::toString()
    { 
      std::stringstream s;
      s << static_cast<int>(type) << " " << lexeme << " " << literal; 
      return s.str();
    }