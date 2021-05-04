#include "ASTVariableExpr.h"
#include "ASTVisitor.h"

void ASTVariableExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

std::ostream& ASTVariableExpr::print(std::ostream &out) const {
  out << getName();
  return out;
}

ASTNode* ASTVariableExpr::instantiate() const {
  return new ASTVariableExpr(this->NAME);
}
