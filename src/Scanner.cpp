#include "Scanner.hpp"
#include "Token.hpp"
#include <cstddef>
#include "Lox.hpp"

Scanner::Scanner(const std::string& Source) 
        : Source{Source}  
        {
        }

std::vector <Token> Scanner::scanTokens()
{
  while (!isatEnd())
  {
    start = current;
    scanToken();
  }

  // Add EOF token to the end
  tokens.push_back(Token(TokenType::_EOF, NULL, NULL, line));
  return tokens;
} 

bool Scanner::isatEnd()
{
  return current >= Source.length();
}

void Scanner::scanToken() 
{
    char c = advance();

    switch (c) 
    {
      case '(': addToken(TokenType::LEFT_PAREN);  break;
      case ')': addToken(TokenType::RIGHT_PAREN); break;
      case '{': addToken(TokenType::LEFT_BRACE);  break;
      case '}': addToken(TokenType::RIGHT_BRACE); break;
      case ',': addToken(TokenType::COMMA);       break;
      case '.': addToken(TokenType::DOT);         break;
      case '-': addToken(TokenType::MINUS);       break;
      case '+': addToken(TokenType::PLUS);        break;
      case ';': addToken(TokenType::SEMICOLON);   break;
      case '*': addToken(TokenType::STAR);        break; 

      case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
      case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
      case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
      case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;

      default:
        Lox::error(line, "Unexpected character");
        break;
    }

  }

void Scanner::addToken(TokenType TokenType, std::string literal)
{
  int end = (current == start) ? 1 : current - start;
  std::string lexeme = Source.substr(start, end);

  tokens.push_back(Token(TokenType, std::move(lexeme), std::move(literal), line));
}

char Scanner::advance()
{
    return Source.at(current++);
}

bool Scanner::match(char expected)
{
  if (isatEnd())
     return false;
  if (Source.at(current) != expected)
     return false;
  
  current++;
  return true;

}