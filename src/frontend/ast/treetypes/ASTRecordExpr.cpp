#include "ASTRecordExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTFieldExpr*> ASTRecordExpr::getFields() const {
  return rawRefs(FIELDS);
}

void ASTRecordExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto f : getFields()) {
      f->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

std::ostream& ASTRecordExpr::print(std::ostream &out) const {
  out << "{";
  bool skip = true;
  for (auto &f : getFields()) {
    if (skip) {
      skip = false;
      out << *f;
      continue;
    }
    out << "," << *f;
  }
  out << "}";
  return out;
}

ASTNode* ASTRecordExpr::instantiate() const {
  std::vector<std::unique_ptr<ASTFieldExpr>> fields;
  for (auto& field : this->FIELDS) {
    fields.push_back(
      std::unique_ptr<ASTFieldExpr>(
        static_cast<ASTFieldExpr*>(field->instantiate())));
  }

  return new ASTRecordExpr(std::move(fields));
}
