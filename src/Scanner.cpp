#include "Scanner.hpp"
#include "Token.hpp"
#include <cctype>
#include <string>
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

  tokens.emplace_back(TokenType::_EOF, "", std::monostate{}, line);
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
        break;
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
        else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
        {
          identifier();
        }
        else
        {
          Lox::error(line, "Unexpected character");
        }
        break;
    }

}

// void Scanner::addToken(TokenType TokenType)
// {
//   // assign type as std::monostate
//   LoxLiteral t = {};
//   addToken(TokenType, std::move(t));
// }

void Scanner::addToken(TokenType TokenType, LoxLiteral&& literal)
{
  std::string lexeme = Source.substr(start, (current - start));

  tokens.emplace_back(TokenType, std::move(lexeme), std::move(literal), line);
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

  std::string str_literal = Source.substr(start+1, (current - start - 2));
  addToken(TokenType::STRING, std::move(str_literal));
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

  addToken(TokenType::NUMBER, std::stod(Source.substr(start, current - start)));
}

void Scanner::identifier()
{
  while (std::isalnum(peek()) || peek() == '_') advance();

  std::string text = Source.substr(start, (current - start));
  
  auto type = keywords.find(text);

  if (type == keywords.end())
      addToken(TokenType::IDENTIFIER);
  else
      addToken(type->second);
}