#pragma once
#include "Expr.hpp"
#include <sstream>
#include <vector>

struct Printer : public Visitor {
    
    std::stringstream s;

    std::string print(Expr& expr);
        
    void visit( Binary& expr) override; 
    void visit(Literal& expr) override;
    void visit(Grouping& expr) override;
    void visit(Unary& expr) override;

    void parenthesize(const std::string& name, std::vector<std::reference_wrapper< Expr>>& exprs);
};

