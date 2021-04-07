#include "ASTAssignStmt.h"
#include "ASTVisitor.h"

void ASTAssignStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLHS()->accept(visitor);
    getRHS()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTAssignStmt::print(std::ostream &out) const {
  out << *getLHS() << " = " << *getRHS() << ";";
  return out;
}

ASTNode* ASTAssignStmt::instantiate() const {
  return new ASTAssignStmt(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->LHS->instantiate())),
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->RHS->instantiate())));
}
