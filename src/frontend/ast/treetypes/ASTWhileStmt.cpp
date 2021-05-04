#include "ASTWhileStmt.h"
#include "ASTVisitor.h"

void ASTWhileStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getCondition()->accept(visitor);
    getBody()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTWhileStmt::print(std::ostream &out) const {
  out << "while (" << *getCondition() << ") " << *getBody();
  return out;
}

ASTNode* ASTWhileStmt::instantiate() const {
  return new ASTWhileStmt(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->COND->instantiate())),
    std::unique_ptr<ASTStmt>(
      static_cast<ASTStmt*>(this->BODY->instantiate())));
}
