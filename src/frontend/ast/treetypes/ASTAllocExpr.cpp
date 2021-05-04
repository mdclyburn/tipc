#include "ASTAllocExpr.h"
#include "ASTVisitor.h"

void ASTAllocExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getInitializer()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTAllocExpr::print(std::ostream &out) const {
  out << "alloc " << *getInitializer();
  return out;
}

ASTNode* ASTAllocExpr::instantiate() const {
  ASTAllocExpr* const a = new ASTAllocExpr(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->INIT->instantiate())));

  return a;
}
