#pragma once

#include "ASTExpr.h"

/*! \brief Class for numeric literals.
 */
class ASTNumberExpr : public ASTExpr {
  int VAL;
public:
  ASTNumberExpr(int VAL) : VAL(VAL) {}
  int getValue() const { return VAL; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
  ASTNode* instantiate() const override { return new ASTNumberExpr(this->VAL); }

protected:
  std::ostream& print(std::ostream &out) const override;
};
