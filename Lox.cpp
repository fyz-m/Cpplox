#include "Lox.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Scanner.hpp"
#include "Token.hpp"

  void Lox::runFile(const std::string& file) 
  {
    std::ifstream input_file{file, std::ios::binary | std::ios::in};

    if (!input_file) {
      throw std::runtime_error("Could not open file: " + file);
    }

    std::stringstream buffer;

    buffer << input_file.rdbuf();

    run(buffer.str());

    if (hadError)
       throw std::runtime_error("65");
           

  }

  void Lox::runPrompt()
  {

  }

  void Lox::run(const std::string& source)
  {
    Scanner scanner =  Scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    
    for (const auto &x : tokens)
    {
      std::cout << x << std::endl;
    }
  }

   void Lox::error(int line, std::string_view message)
  {
    report(line, "aa" , message);
  }

  void Lox::report(int line, std::string where, std::string_view message)
  {
    std::cout << "[line " << line << " ] Error" << where << ": " << message << std::endl;
    hadError = true;
  }

  