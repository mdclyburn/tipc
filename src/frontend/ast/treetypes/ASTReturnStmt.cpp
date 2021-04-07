#include "ASTReturnStmt.h"
#include "ASTVisitor.h"

void ASTReturnStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTReturnStmt::print(std::ostream &out) const {
  out << "return " << *getArg() << ";";
  return out;
}

ASTNode* ASTReturnStmt::instantiate() const {
  return new ASTReturnStmt(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->ARG->instantiate())));
}
