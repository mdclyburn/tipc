#include "ASTRefExpr.h"
#include "ASTVisitor.h"

void ASTRefExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getVar()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTRefExpr::print(std::ostream &out) const {
  out << "&" << *getVar();
  return out;
}

ASTNode* ASTRefExpr::instantiate() const {
  return new ASTRefExpr(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->VAR->instantiate())));
}
