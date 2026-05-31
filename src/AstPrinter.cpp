#include "AstPrinter.hpp"
#include "Expr.hpp"
#include "Token.hpp"
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <variant>


    std::string Printer::print(Expr& expr) {
        s.clear();   // Clear if called again
        expr.accept(*this);
        return s.str();
    }

    void Printer::visit(Binary& expr) {
        
        parenthesize(expr.operator_.lexeme, {expr.left.get(), expr.right.get()});
    } 

    void Printer::visit(Literal& expr) {

        if (expr.value.index() == 0)
        {
            s << "nil";
        }
        else if (auto ptr = std::get_if<float>(&expr.value))
        {
            s << *ptr;
        }
        else if (auto ptr = std::get_if<std::string>(&expr.value))
        {
            s << *ptr;
        }
    }   

    void Printer::visit(Grouping& expr) {

        parenthesize("group", {expr.expression.get()});
    }

    void Printer::visit(Unary& expr) {
        
        parenthesize(expr.operator_.lexeme, {expr.expression.get()});
    }

    void Printer::parenthesize(const std::string& name, std::initializer_list<Expr*> exprs) {

        s << "(" << name;

        for (auto expr : exprs) {
            s << " ";
            expr->accept(*this);
        }

        s << ")";
    }

// int main() {

//     auto unexp = std::make_unique<Unary>(

//         std::make_unique<Literal>(static_cast<float>(123)),
//         Token(TokenType::MINUS, "-", std::monostate{}, 1)
//     );

//     auto grexp = std::make_unique<Grouping>(
//         std::make_unique<Literal>(static_cast<float>(45.67))
//     );


//     std::unique_ptr<Expr>  expression = std::make_unique<Binary>(

//         std::move(unexp),
//         Token(TokenType::MINUS, "-", std::monostate{}, 1),
//         std::move(grexp)
//     );

//     Printer printer;
//     std::cout << printer.print(*expression) << std::endl;
// }