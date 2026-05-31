#include "Parser.hpp"
#include <vector>


Parser::Parser(const std::vector<Token>&& tokens)
        : tokens{std::move(tokens)} {}