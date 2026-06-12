#include "Parser.hpp"
#include "Expr.hpp"
#include "Lox.hpp"
#include "Stmt.hpp"
#include "Token.hpp"
#include <cerrno>
#include <memory>
#include <utility>
#include <variant>
#include <vector>


Parser::Parser(std::vector<Token>&& tokens)
       : tokens{std::move(tokens)} 
       {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    
    std::vector<std::unique_ptr<Stmt>> statements{};
    
    while(!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::declaration() {

    try { 
        if (match({TokenType::VAR})) 
            return varDeclaration();
        if (match({TokenType::LEFT_BRACE}))
            return std::make_unique<BlockStmt>(block());

        return statement();

    } catch (const ParseError& e) {
        synchronize();
        return nullptr;   
    }
}   

std::unique_ptr<VarDeclarationStmt> Parser::varDeclaration() {

    Token& name = consume(TokenType::IDENTIFIER, "Variable declaration requires an indentifier");
    std::unique_ptr<Expr> initializer {nullptr};

    if (match({TokenType::EQUAL}))
        initializer = expression();

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");

    return std::make_unique<VarDeclarationStmt>(std::move(initializer), std::move(name)); 
}

std::unique_ptr<Stmt> Parser::statement() {

    if(match({TokenType::PRINT})) return printStatement();
    if(match({TokenType::IF})) return ifStatement();
    
    return expressionStatement();
}

std::unique_ptr<ExpressionStmt> Parser::expressionStatement() {

    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    
    return std::make_unique<ExpressionStmt>(std::move(value));
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {

    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect closing '}'.");
    return statements;
}

std::unique_ptr<PrintStmt> Parser::printStatement() {

    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");

    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<ifStmt> Parser::ifStatement() {

    auto condition = expression();
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'if' condition.");

    auto thenBranch = std::make_unique<BlockStmt>(block());

    std::unique_ptr<BlockStmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        consume(TokenType::LEFT_BRACE, "Expect '{' after 'else'.");
        elseBranch = std::make_unique<BlockStmt>(block());
    }
    return std::make_unique<ifStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Expr> Parser::expression() {
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment() {

    // Variable being assigned is parsed as an expression
    auto expr = logicalOr();
    
    if (match({TokenType::EQUAL})) {
        auto equal = std::move(previous());
        auto value = assignment();

        if (auto var = dynamic_cast<Variable*>(expr.get())) {
            return std::make_unique<Assignment>(std::move(var->name), std::move(value));
        }

        // If receiver of assignment is not a variable
        error(equal, "Invalid assignment target"); 
    }   

    return expr;
}

std::unique_ptr<Expr> Parser::logicalOr() {

    auto expr = logicalAnd();

    if (match({TokenType::OR})) {
        auto rhs = logicalAnd();
        Token& operator_ = previous();
        return std::make_unique<Logical>(std::move(expr), std::move(operator_), std::move(rhs));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::logicalAnd() {
    
    auto expr = equality();

    if (match({TokenType::AND})) {
        auto rhs = equality();
        Token& operator_ = previous();
        return std::make_unique<Logical>(std::move(expr), std::move(operator_), std::move(rhs));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    
    // Left child node of the binary AST node
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL})) {
          Token operator_ = std::move(previous()); 
          auto right = comparison(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {

    // Left child node of the binary AST node
    auto expr = term();

    while (match({
                             TokenType::GREATER,
                             TokenType::GREATER_EQUAL,
                             TokenType::LESS,
                             TokenType::LESS_EQUAL
                                 })) {

          Token operator_ = std::move(previous()); 
          auto right = term(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::term() {

    // Left child node of the binary AST node
    auto expr = factor();

    while (match({
                             TokenType::PLUS,
                             TokenType::MINUS
                                 })) {

          Token operator_ = std::move(previous()); 
          auto right = factor(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }
    
    return expr;

}

std::unique_ptr<Expr> Parser::factor() {

    // Left child node of the binary AST node
    auto expr = unary();

    while (match({
                             TokenType::STAR,
                             TokenType::SLASH
                                 })) {

          Token operator_ = std::move(previous()); 
          auto right = unary(); // right child node
          expr = std::make_unique<Binary>(std::move(expr), std::move(operator_), std::move(right));                             
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {

    if (match({TokenType::BANG, TokenType::MINUS})) {
        auto operator_ = std::move(previous());
        auto expr = unary();
        return std::make_unique<Unary>(std::move(expr), std::move(operator_));
    }
    
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {

    if (match({TokenType::TRUE})) 
        return std::make_unique<Literal>(true);

    if (match({TokenType::FALSE})) 
        return std::make_unique<Literal>(false);

    if (match({TokenType::NIL})) 
        return std::make_unique<Literal>(std::monostate{});

    if (match({TokenType::STRING, TokenType::NUMBER})) 
        return std::make_unique<Literal>(std::move(previous().literal));

    if (match({TokenType::IDENTIFIER}))
        return std::make_unique<Variable>(std::move(previous()));

    if (match({TokenType::LEFT_PAREN})) {     
        
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");  
        return std::make_unique<Grouping>(std::move(expr));
    }
    
    throw error(peek(), "Expect expression.");
}


bool Parser::match(std::initializer_list<TokenType> expected_types) {

  for (auto expected_type : expected_types) {
    
      if (check(expected_type)) {
         advance();
         return true;
      }
  }
    return false;
}

bool Parser::check(TokenType expected_type) {
    return peek().type == expected_type;
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::_EOF;
}

Token& Parser::peek() {
    return tokens[current];
}
 
Token& Parser::previous() {
    return tokens[current-1];
}

Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}  

Token& Parser::consume(TokenType type, std::string_view error_message) {
    if (check(type)) return advance();
    throw error(peek(), error_message);
}

ParseError Parser::error(Token& token, std::string_view error_message) {
    Lox::error(token, error_message);
    return ParseError();
}

void Parser::synchronize() {
    advance();

    while(!isAtEnd()) {
        if(previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
            return;
            default: break;
        }
        advance();
    }
}

