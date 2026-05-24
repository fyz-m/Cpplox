#pragma once
#include <string>


class Lox {

  private:

   inline static bool hadError = false;

  public:

  private:

    static void report(int line, std::string where, std::string_view message);
     
  public:

    void runFile(const std::string& file);
  
    void runPrompt();  

    void run(const std::string& source);
  
    static void error(int line, std::string_view message);
    
   
};