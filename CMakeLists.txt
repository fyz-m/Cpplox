cmake_minimum_required(VERSION 3.28.3)

project(lox LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpplox src/main.cpp 
                      src/Lox.cpp
                      src/Lox.hpp
                      src/Scanner.cpp
                      src/Scanner.hpp
                      src/Token.cpp
                      src/Token.hpp
                      src/Expr.hpp
                      src/AstPrinter.cpp 
                      src/AstPrinter.hpp
                      src/Parser.cpp                                           
                      src/Parser.hpp
                      src/Interpreter.cpp
                      src/Interpreter.hpp                                           
)

target_compile_options(cpplox  PRIVATE -Wall -Wextra -Wpedantic  -g -O0 )
