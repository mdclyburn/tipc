#include "PolymorphicIdentifierVisitor.h"

#include "ASTHelper.h"
#include "SymbolTable.h"
#include "catch.hpp"
#include <iostream>
#include <sstream>

TEST_CASE("PolymorphicIdentifierVisitor: poly function id", "[PolymorphicIdentifierVisitor]") {
	std::stringstream program;
    program << R"(poly(a, b) {
 return a(b);
}

nonpoly(a) {
 return a+1;
}
    )";

    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());

    PolymorphicIdentifierVisitor visitor(symbols.get());
    ast->accept(&visitor);
    
    REQUIRE(visitor.polymorphicFunctions().count("poly") == 1);
    REQUIRE(visitor.polymorphicFunctions().count("nonpoly") == 0);
}

TEST_CASE("PolymorphicIdentifierVisitor: ignore recursive poly", "[PolymorphicIdentifierVisitor]") {
	std::stringstream program;
    program << R"(poly_rec(b, k) {
 var a;
 a = b(k);
 return poly_rec(b, a);
}

poly_no_rec(a) {
 return *a;
}
    )";

    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());

    PolymorphicIdentifierVisitor visitor(symbols.get());
    ast->accept(&visitor);
    
    REQUIRE(visitor.polymorphicFunctions().count("poly_rec") == 0);
    REQUIRE(visitor.polymorphicFunctions().count("poly_no_rec") == 1);
}