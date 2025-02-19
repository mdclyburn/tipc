#include "catch.hpp"
#include "TipInt.h"
#include "TipVar.h"
#include "TipAlpha.h"
#include <vector>

TEST_CASE("TipVar: test TipVars objects with the same underlying node are equal" "[TipVar]") {
    ASTNumberExpr n(42);
    TipVar var(&n);
    TipVar var2(&n);
    REQUIRE(var == var2);
}

TEST_CASE("TipVar: test TipVars objects with different underlying node are not equal" "[TipVar]") {
    ASTNumberExpr n(99);
    ASTNumberExpr n1(99);
    TipVar var(&n);
    TipVar var2(&n1);
    REQUIRE_FALSE(var == var2);
}

TEST_CASE("TipVar: test TipVar is a TipType" "[TipVar]") {
    ASTNumberExpr n(42);
    TipVar var(&n);
    REQUIRE_FALSE(nullptr == dynamic_cast<TipType *>(&var));
}

TEST_CASE("TipVar: only tipalpha vars have free" "[TipVar]") {
    ASTNumberExpr n(42);
    TipVar var(&n);
    REQUIRE_FALSE(var.containsFreeVariable());
}