#include "ASTErrorStmt.h"
#include "ASTVisitor.h"

void ASTErrorStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTErrorStmt::print(std::ostream &out) const {
  out << "error " << *getArg() << ";";
  return out;
}

ASTNode* ASTErrorStmt::instantiate() const {
  return new ASTErrorStmt(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->ARG->instantiate())));
}
