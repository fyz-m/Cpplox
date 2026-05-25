#include "Token.hpp"
#include <sstream>


  Token::Token(
          TokenType type, 
          std::string&& lexeme, 
          literaltypes literal,
          int line
          )
          : type{type}, 
            lexeme{lexeme},
            literal{literal}, 
            line{line}
            {
            }
          
    std::string Token::toString()
    { 
      std::stringstream s;
      // s << static_cast<int>(type) << " " << lexeme << " " << literal; 
      return s.str();
    }