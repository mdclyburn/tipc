#include "ASTIfStmt.h"
#include "ASTVisitor.h"

void ASTIfStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getCondition()->accept(visitor);
    getThen()->accept(visitor);
    if (getElse() != nullptr) {
      getElse()->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

std::ostream& ASTIfStmt::print(std::ostream &out) const {
  out << "if (" << *getCondition() << ") ";
  out << *getThen();
  if (getElse() != nullptr) {
     out << " else " << *getElse();
  }
  return out;
}

ASTNode* ASTIfStmt::instantiate() const {
  return new ASTIfStmt(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->COND->instantiate())),
    std::unique_ptr<ASTStmt>(
      static_cast<ASTStmt*>(this->THEN->instantiate())),
    std::unique_ptr<ASTStmt>(
      static_cast<ASTStmt*>(this->ELSE->instantiate()))); // <-- are you going to cause a problem later?
}
