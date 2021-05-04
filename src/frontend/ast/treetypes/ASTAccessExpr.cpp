#include "ASTAccessExpr.h"
#include "ASTVisitor.h"

void ASTAccessExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getRecord()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTAccessExpr::print(std::ostream &out) const {
  out << "(" << *getRecord() << "." << getField() << ")";
  return out;
}

ASTNode* ASTAccessExpr::instantiate() const {
  return new ASTAccessExpr(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->RECORD->instantiate())),
    this->FIELD);
}
