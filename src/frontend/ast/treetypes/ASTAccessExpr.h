#pragma once

#include "ASTExpr.h"

/*! \brief Class for a record field access
 */
class ASTAccessExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> RECORD;
  std::string FIELD;
public:
  ASTAccessExpr(std::unique_ptr<ASTExpr> RECORD, const std::string &FIELD)
      : RECORD(std::move(RECORD)), FIELD(FIELD) {}
  std::string getField() const { return FIELD; }
  ASTExpr* getRecord() const { return RECORD.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
  ASTNode* instantiate() const override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
