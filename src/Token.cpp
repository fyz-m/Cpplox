#include "Token.hpp"
#include <sstream>


  Token::Token(
          TokenType type, 
          std::string&& lexeme, 
          literaltypes&& literal,
          int line
          )
          : type{type}, 
            lexeme{std::move(lexeme)},
            literal{std::move(literal)}, 
            line{line}
            {
            }
          
    std::string Token::toString() const
    { 
      std::stringstream s;
      s << lexeme << " "; 
      return s.str();
    }