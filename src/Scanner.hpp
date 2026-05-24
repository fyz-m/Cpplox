#pragma once
#include <string>
#include <vector>
#include "Token.hpp"


class Scanner {

  private:
    
    const std::string& Source;

    std::vector <Token> tokens;

    int start {0};
    int current {0};
    int line {1};

  public:

    Scanner(const std::string& Source);

    std::vector <Token> scanTokens();

  private:

    bool isatEnd();

    void scanToken();

    char advance();

    // Adds a token 
    void addToken(
          TokenType TokenType, 
          std::string literal = NULL
          );

    bool match(char expected);
};