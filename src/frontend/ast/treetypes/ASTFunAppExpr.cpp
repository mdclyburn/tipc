#include "ASTFunAppExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTExpr*> ASTFunAppExpr::getActuals() const {
  return rawRefs(ACTUALS);
}

void ASTFunAppExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getFunction()->accept(visitor);
    for (auto a : getActuals()) {
      a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

std::ostream& ASTFunAppExpr::print(std::ostream &out) const {
  out << *getFunction() << "(";
  bool skip = true;
  for (auto &arg : getActuals()) {
    if (skip) {
      skip = false;
      out << *arg;
      continue;
    }
    out << ", " << *arg;
  }
  out << ")";
  return out;
}

ASTNode* ASTFunAppExpr::instantiate() const {
  std::vector<std::unique_ptr<ASTExpr>> actuals;
  for (auto& actual : this->ACTUALS) {
    actuals.push_back(
      std::unique_ptr<ASTExpr>(
        static_cast<ASTExpr*>(actual->instantiate())));
  }

  return new ASTFunAppExpr(
    std::unique_ptr<ASTExpr>(
      static_cast<ASTExpr*>(this->FUN->instantiate())),
    std::move(actuals));
}
