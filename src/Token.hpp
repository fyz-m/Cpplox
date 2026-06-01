#pragma once
#include <string>
#include <unordered_map>
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
  
// Types that valid for literals in the Lox language
// monostate == null
using literaltypes = std::variant<std::monostate, std::string, float, bool>;

class Token {

  public:

    const TokenType type;

    const std::string lexeme;

    literaltypes literal;

    const int line;
    
  public:


    Token(
          TokenType type, 
          std::string&& lexeme, 
          literaltypes&& literal,
          int line
         );

    std::string toString() const;
    
};

const std::unordered_map<std::string, TokenType> keywords =  
    {
      {"and", TokenType::AND},
      {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},
      {"false", TokenType::FALSE},
      {"for", TokenType::FOR},
      {"fun", TokenType::FUN},
      {"if", TokenType::IF},
      {"nil", TokenType::NIL},
      {"or", TokenType::OR},
      {"print", TokenType::PRINT},
      {"return", TokenType::RETURN},
      {"super", TokenType::SUPER},
      {"this", TokenType::THIS},
      {"true", TokenType::TRUE},
      {"var", TokenType::VAR},
      {"while", TokenType::WHILE},
    };

