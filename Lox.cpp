#include "Lox.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

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
    Scanner scanner = new Scanner(source);
    std::vector<std::string> tokens = scanner.scanTokens();
    
    for (const auto &x : tokens)
    {
      std::cout << x << std::endl;
    }
  }

  void Lox::error(int line, std::string &message)
  {
    report(line, "" ,message);
  }

  void Lox::report(int line, std::string &where, std::string &message)
  {
  std::cout << "[line " << line << " ] Error" << where << ": " << message << std::endl;
  hadError = true;
  }

  