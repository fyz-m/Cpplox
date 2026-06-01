#pragma once
#include "Token.hpp"
#include <string>


class Lox {

  private:

   inline static bool hadError = false;

  public:

  private:

    static void report(int line, std::string_view where, std::string_view message);

    static void report(int line, std::string_view message);
     
  public:

    static void runFile(const std::string& file);
  
    static void runPrompt();  

    static void run(const std::string& source);
  
    static void error(Token& token, std::string_view message);

    static void error(int line, std::string_view message);
    
   
};