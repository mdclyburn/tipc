#include "ASTDeRefExpr.h"
#include "ASTVisitor.h"

void ASTDeRefExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getPtr()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTDeRefExpr::print(std::ostream &out) const {
  out << "(*" << *getPtr() << ")";
  return out;
}

ASTNode* ASTDeRefExpr::instantiate() const {
  return new ASTDeRefExpr(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->PTR->instantiate())));
}
