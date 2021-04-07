#include "ASTOutputStmt.h"
#include "ASTVisitor.h"

void ASTOutputStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTOutputStmt::print(std::ostream &out) const {
  out << "output " << *getArg() << ";";
  return out;
}

ASTNode* ASTOutputStmt::instantiate() const {
  return new ASTOutputStmt(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->ARG->instantiate())));
}
