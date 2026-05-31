#include "AstPrinter.hpp"
#include "Expr.hpp"
#include "Token.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

    std::string Printer::print(Expr& expr)
    {
        expr.accept(*this);
        return s.str();
    }

    void Printer::visit(Binary& expr) {

        std::vector<std::reference_wrapper< Expr>> child_nodes;
        child_nodes.push_back(*(expr.left));
        child_nodes.push_back(*(expr.right));
    
        
        parenthesize(expr.operator_.lexeme, child_nodes);
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

        std::vector<std::reference_wrapper<Expr>> child_nodes;
        child_nodes.push_back(*(expr.expression));

        parenthesize("Group", child_nodes);
    }

    void Printer::visit(Unary& expr) {

        std::vector<std::reference_wrapper<Expr>> child_nodes;
        child_nodes.push_back(*(expr.expression));
        
        parenthesize(expr.operator_.lexeme, child_nodes);
    }

    void Printer::parenthesize(const std::string& name, std::vector<std::reference_wrapper<Expr>>& exprs) {

        s << "(" << name;

        for (Expr& expr : exprs) {
            s << " ";
            expr.accept(*this);
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


//     auto  expression = std::make_unique<Binary>(

//         std::move(unexp),
//         Token(TokenType::MINUS, "-", std::monostate{}, 1),
//         std::move(grexp)
//     );

//     Printer printer;
//     std::cout << printer.print(*expression) << std::endl;
// }