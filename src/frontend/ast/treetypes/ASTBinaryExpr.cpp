#include "ASTBinaryExpr.h"
#include "ASTVisitor.h"

void ASTBinaryExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLeft()->accept(visitor);
    getRight()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTBinaryExpr::print(std::ostream &out) const {
  out << "(" << *getLeft() << getOp() << *getRight() << ")";
  return out;
}

ASTNode* ASTBinaryExpr::instantiate() const {
  return new ASTBinaryExpr(
    this->OP,
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->LEFT->instantiate())),
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->RIGHT->instantiate())));
}
