#include "catch.hpp"
#include "TipInt.h"
#include "TipRef.h"
#include "TipFunction.h"
#include "ASTNumberExpr.h"
#include "TipAlpha.h"
#include <sstream>
#include <iostream>

TEST_CASE("TipFunction: Test getters" "[TipFunction]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipRef>(std::make_shared<TipInt>());
    TipFunction tipFunction(params, ret);

    REQUIRE(2 == tipFunction.getParams().size());
    REQUIRE(*ret == *tipFunction.getReturnValue());
}

TEST_CASE("TipFunction: Test equality" "[TipFunction]") {
    std::vector<std::shared_ptr<TipType>> paramsA {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto retA = std::make_shared<TipInt>();
    TipFunction tipFunctionA(paramsA, retA);

    SECTION("Equal when arguments and return value are of same type and length") {
        std::vector<std::shared_ptr<TipType>> paramsB {
                std::make_shared<TipInt>(),
                std::make_shared<TipInt>()
        };
        auto retB = std::make_shared<TipInt>();
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA == tipFunctionB);
    }

    SECTION("Not equal when arguments differ by length") {
        std::vector<std::shared_ptr<TipType>> paramsB {std::make_shared<TipInt>()};
        auto retB = std::make_shared<TipInt>();
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA != tipFunctionB);
    }

    SECTION("Not equal when arguments differ by type") {
        std::vector<std::shared_ptr<TipType>> paramsB {
            std::make_shared<TipInt>(),
            std::make_shared<TipRef>(std::make_shared<TipInt>())
        };
        auto retB = std::make_shared<TipInt>();
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA != tipFunctionB);
    }

    SECTION("Not equal when return values differ by type") {
        std::vector<std::shared_ptr<TipType>> paramsB {
                std::make_shared<TipInt>(),
                std::make_shared<TipInt>()
        };
        auto retB = std::make_shared<TipRef>(std::make_shared<TipInt>());
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA != tipFunctionB);
    }
}

TEST_CASE("TipFunction: Test output stream" "[TipFunction]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipRef>(std::make_shared<TipInt>());
    TipFunction tipFunction(params, ret);

    auto expectedValue = "(int,int) -> &int";
    std::stringstream stream;
    stream << tipFunction;
    std::string actualValue = stream.str();

    REQUIRE(expectedValue == actualValue);
}

TEST_CASE("TipFunction: Test containsfreevariable depends on content" "[TipFunction]") {
    ASTNumberExpr num(13);
    std::vector<std::shared_ptr<TipType>> params1 { std::make_shared<TipInt>() };
    std::vector<std::shared_ptr<TipType>> params2 { std::make_shared<TipInt>(), std::make_shared<TipAlpha>(&num) };
    auto ret = std::make_shared<TipInt>();
    TipFunction tipFunction1(params1, ret);
    TipFunction tipFunction2(params2, ret);

    REQUIRE_FALSE(tipFunction1.containsFreeVariable());
    REQUIRE(tipFunction2.containsFreeVariable());
}
