#include "PolymorphicIdentifierVisitor.h"

#include "ASTHelper.h"
#include "SymbolTable.h"
#include "catch.hpp"
#include <iostream>
#include <sstream>

TEST_CASE("PolymorphicIdentifierVisitor: poly function id", "[PolymorphicIdentifierVisitor]") {
	std::stringstream program;
    program << R"(recpoly(a, b) {
 return a(b);
}

poly(a, b) {
  return a == b;
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
    REQUIRE(visitor.polymorphicFunctions().count("recpoly") == 0);
    REQUIRE(visitor.polymorphicFunctions().count("nonpoly") == 0);
}
