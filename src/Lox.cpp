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
#include "Parser.hpp"

//TODO:
// Update parser constructor argument to scanTokens() for compiler RVO
  void Lox::runFile(const std::string& file) 
  {
    std::ifstream input_file{file, std::ios::binary | std::ios::in};

    if (!input_file) {
      throw std::runtime_error("Could not open file: " + file);
    }

    std::stringstream buffer;

    buffer << input_file.rdbuf();

    run(buffer.str());

    // if (hadError)
    //    throw std::runtime_error("65");
    // if (hadRuntimeError)
    //    throw std::runtime_error("70");           

  }

  void Lox::runPrompt()
  {

  }

  void Lox::run(const std::string& source)
  {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    Parser parser(std::move(tokens));
    auto statements  = parser.parse();
    
    // if syntax error
    if (hadError) return;

    interpreter.interpret(statements);
  }

   void Lox::runtimeError(const RuntimeError& e) {

      std::cout << e.what() << "\n[ line " << e.token.line << " ]" << std::endl;   
      hadRuntimeError = true; 
   }

   void Lox::error(Token& token, std::string_view message)
   {
    if (token.type == TokenType::_EOF)
      report(token.line, " at end ", message);
    else
      report(token.line, " at '" + token.lexeme + "' ", message);
   }

  void Lox::error(int line, std::string_view message) 
  {
    report(line, message);  
  }

  void Lox::report(int line, std::string_view where, std::string_view message)
  {
    std::cout << "[ line " << line << " ] Error" << where << ": " << message << std::endl;
    hadError = true;
  }

  void Lox::report(int line, std::string_view message)
  {
    std::cout << "[ line " << line << " ] Error"  << ": " << message << std::endl;
    hadError = true;
  }


  