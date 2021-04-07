#include "ASTFieldExpr.h"
#include "ASTVisitor.h"

void ASTFieldExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getInitializer()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTFieldExpr::print(std::ostream &out) const {
  out << getField() << ":" << *getInitializer();
  return out;
}

ASTNode* ASTFieldExpr::instantiate() const {
  return new ASTFieldExpr(
    this->FIELD,
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(
        this->INIT->instantiate())));
}
