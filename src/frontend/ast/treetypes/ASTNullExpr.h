#pragma once

#include "ASTExpr.h"

/*! \brief Class for a null expression
 */
class ASTNullExpr : public ASTExpr {
public:
  ASTNullExpr() {}
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
  ASTNode* instantiate() const override { return new ASTNullExpr(); }

protected:
  std::ostream& print(std::ostream &out) const override;
};
