#include "Parser.hpp"
#include "Expr.hpp"
#include "Lox.hpp"
#include "Stmt.hpp"
#include "Token.hpp"
#include <cerrno>
#include <ctime>
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
        if (match({TokenType::FUN}))
            return function("function");
        if (match({TokenType::VAR})) 
            return varDeclaration();

        return statement();

    } catch (const ParseError& e) {
        synchronize();
        return nullptr;   
    }
}   

std::unique_ptr<functionStmt> Parser::function(const std::string& kind) {

    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " declaration.");

    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255)
                error(peek(), "Maximum parameter count exceeded (254)");
            parameters.push_back(
                consume(TokenType::IDENTIFIER, "Expect parameter name")
            );
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Missing closing brace ')' after " + kind + " parameters.");

    consume(TokenType::LEFT_BRACE, "Expect '{' after " + kind + " arguments (before " + kind + " body).");
    auto functionBody = block();

    return std::make_unique<functionStmt>(
        std::move(name),
        std::move(parameters),
        std::move(functionBody)
    );
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

    if (match({TokenType::PRINT}))   return printStatement();
    if (match({TokenType::IF}))      return ifStatement();
    if (match({TokenType::WHILE}))   return whileStatement();
    if (match({TokenType::FOR}))     return forStatement();
    if (match({TokenType::BREAK}))   return breakStatement();

    if (match({TokenType::LEFT_BRACE}))
        return std::make_unique<BlockStmt>(block());
    
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::forStatement() {

    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) 
        initializer = nullptr;
    else if (match({TokenType::VAR})) 
        initializer = varDeclaration();
    else 
        initializer = expressionStatement();

    std::unique_ptr<Expr> for_condition {nullptr};
    if (!check(TokenType::SEMICOLON))
        for_condition = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after for-loop condition.");


    std::unique_ptr<Expr> increment {nullptr};
    if (!check(TokenType::RIGHT_PAREN))
        increment = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for-loop clauses.");

    // Temporarily use a whileStmt ptr so we can handle break statements in the for-loop body
    auto whileLoop = std::make_unique<whileStmt>(nullptr, nullptr);
    auto prevloop = enclosingLoop;
    enclosingLoop = whileLoop.get();

    // Parse for-loop body
    auto body = statement();
    enclosingLoop = prevloop;

    // Convert the for-loop into a while-loop:
    // it becomes a blockStmt node where the first statement (in the vector of stmts)
    // is the initializer and the second statement is a whileStmt.
    // The condition of the for-loop becomes the condition of the whileStmt
    // and the body of the for-loop + incrementExpr becomes the body of the whileStmt

    // blockStmt:
    // initializer
    // while (condition) {
    //     statement;
    //     increment;
    // }

    if (increment != nullptr) {
        std::vector<std::unique_ptr<Stmt>> v(2);
        v[0] = std::move(body);
        // Convert increment into a stmt node so we can make a blockStmt 
        v[1] = std::make_unique<ExpressionStmt>(std::move(increment));

        // This becomes the body of the while loop
        // containing the body + increment at the end
        body = std::make_unique<BlockStmt>(std::move(v));
    } 

    // If condition omitted it gets set to true
    if (for_condition == nullptr) for_condition = std::make_unique<Literal>(true); 

    whileLoop->condition = std::move(for_condition); 
    whileLoop->bodyStatements = std::move(body);
    
    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Stmt>> v;
        v.push_back(std::move(initializer));
        v.push_back(std::move(whileLoop));
        return std::make_unique<BlockStmt>(std::move(v));
    } else {
        return whileLoop;
    }
        
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

    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect closing ')'.");

    auto thenBranch = statement();

    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }
    return std::make_unique<ifStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<whileStmt> Parser::whileStatement() {

    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect closing ')'.");

    // Create node before parsing body statements because we need to set the parser's 
    // internal state so it knows it is in a loop. This is so we can handle break
    // statements that are not in loops (syntax error).  
    auto whileLoop = std::make_unique<whileStmt>(std::move(condition), nullptr);

    auto prevloop = enclosingLoop;
    enclosingLoop = whileLoop.get();

    whileLoop->bodyStatements = statement();
    enclosingLoop = prevloop;

    return whileLoop; 
}

std::unique_ptr<breakStmt> Parser::breakStatement() {

    if (enclosingLoop == nullptr) {
        error(previous(), "'break' must be inside a for/while loop.");
        return nullptr;
    }
    consume(TokenType::SEMICOLON, "Expect ';' after 'break'.");

    return std::make_unique<breakStmt>();

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
        Token& operator_ = previous();
        auto rhs = logicalAnd();
        return std::make_unique<Logical>(std::move(expr), std::move(operator_), std::move(rhs));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::logicalAnd() {
    
    auto expr = equality();

    if (match({TokenType::AND})) {
        Token& operator_ = previous();
        auto rhs = equality();
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
    
    return functionCall();
}

std::unique_ptr<Expr> Parser::functionCall() {

    auto expr = primary();

    while (true) {
        if (match({TokenType::LEFT_PAREN})) {
            expr = finishCall(std::move(expr));
        }
    }

    return expr;
}

std::unique_ptr<Call> Parser::finishCall(std::unique_ptr<Expr> expr) {

    std::vector<std::unique_ptr<Expr>> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) 
                error(peek(), "Maximum argument count exceeded (254)");
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    consume(TokenType::RIGHT_PAREN, "Expect closing ')' after function arguments.");

    return std::make_unique<Call>(std::move(expr), std::move(previous()), std::move(arguments));
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

