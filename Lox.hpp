#pragma once
#include <string>


class Lox {

  private:

    inline static bool hadError = false;

  public:

  private:

    void report(int line, std::string &where, std::string &message);
     
  public:

    void runFile(const std::string& file);
  
    void runPrompt();  

    void run(const std::string& source);
  
    void error(int line, std::string &message);
    
   
};