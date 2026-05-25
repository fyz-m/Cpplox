#pragma once
#include <string>
#include <variant>

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  _EOF
};

using literaltypes = std::variant<std::monostate, std::string, float>;

class Token {

  public:

    const TokenType type;

    const std::string lexeme;

    const literaltypes literal;

    const int line;

    Token(
          TokenType type, 
          std::string&& lexeme, 
          literaltypes literal,
          int line
         );

    std::string toString();
    
};

