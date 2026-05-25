#include "Scanner.hpp"
#include "Token.hpp"
#include <cstddef>
#include <variant>
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
  // tokens.push_back(Token(TokenType::_EOF, NULL, NULL, line));
  addToken(TokenType::_EOF);
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

      case '/':
      {
        // If comment
        if (match('/'))
        {
          // Consume char until new line 
          while (peek() != '\n' && !isatEnd())
            advance();
        }
        else 
        {
          addToken(TokenType::SLASH);
        }
      }
      
      case ' ':
      case '\t':
      case '\r':
        break;
      
      case '\n':
        line++;
        break;

      case '"': string(); break;


      default:
        if (std::isdigit(c)) 
        {
          number();
        }
        else
        {
          Lox::error(line, "Unexpected character");
        }
        break;
    }

  }

void Scanner::addToken(TokenType TokenType)
{
  // assign type as std::monostate
  literaltypes t = {};
  addToken(TokenType, t);
}

void Scanner::addToken(TokenType TokenType, literaltypes literal)
{
  std::string lexeme = Source.substr(start, (current - start));

  tokens.push_back(Token(TokenType, std::move(lexeme), std::move(literal), line));
}

char Scanner::advance()
{
    // post increment:
    // returns source[current] THEN current += 1
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

char Scanner::peek()
{
  // Return current (unconsumed) char
  if (isatEnd())
    return '\0';

  return Source.at(current);
}

char Scanner::peekNext()
{
  // Return current+1 (unconsumed) char
  if (current+1 >= Source.length())
     return '\0';

  return Source.at(current+1);
}

void Scanner::string()
{
  while (peek() != '"')
  {
    if (isatEnd()) 
    {
      Lox::error(line , "Unterminated string literal");
      return;
    }

    if (peek() == '\n') 
      line++;

    advance();
  }

  // Consume closing "
  advance();

  std::string str_literal = Source.substr(start+1, (current - start - 1));
  addToken(TokenType::STRING, str_literal);
}

void Scanner::number()
{
  while(std::isdigit(peek())) 
        advance();
  
  if (peek() == '.' && std::isdigit(peekNext()))
  {
    // Consume '.'
    advance();
  
    while(std::isdigit(peek())) 
         advance();
  } 

  addToken(TokenType::NUMBER, std::stof(Source.substr(start, current - start)));
}