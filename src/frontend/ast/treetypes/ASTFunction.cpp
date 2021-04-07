#include "ASTFunction.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTDeclNode*> ASTFunction::getFormals() const {
  return rawRefs(FORMALS);
}

std::vector<ASTDeclStmt*> ASTFunction::getDeclarations() const {
  return rawRefs(DECLS);
}

std::vector<ASTStmt*> ASTFunction::getStmts() const {
  return rawRefs(BODY);
}

void ASTFunction::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getDecl()->accept(visitor);
    for (auto p : getFormals()) {
      p->accept(visitor);
    }
    for (auto d : getDeclarations()) {
      d->accept(visitor);
    }
    for (auto s : getStmts()) {
      s->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

//! \brief Print an abbreviated unique string for the function
std::ostream& ASTFunction::print(std::ostream &out) const {
  out << *getDecl() << "(";
  bool skip = true;
  for (auto &p : getFormals()) {
    if (skip) {
      skip = false;
      out << *p;
      continue;
    }
    out << "," << *p;
  }
  out << ") {...}";
  return out;
}

ASTNode* ASTFunction::instantiate() const {
  std::vector<std::unique_ptr<ASTDeclNode>> formals;
  std::vector<std::unique_ptr<ASTDeclStmt>> decls;
  std::vector<std::unique_ptr<ASTStmt>> body;

  for (auto& formal : this->FORMALS)
    formals.push_back(
      std::unique_ptr<ASTDeclNode>(
        static_cast<ASTDeclNode*>(formal->instantiate())));
  for (auto& decl : this->DECLS)
    decls.push_back(
      std::unique_ptr<ASTDeclStmt>(
        static_cast<ASTDeclStmt*>(decl->instantiate())));
  for(auto& stmt : this->BODY)
    body.push_back(
      std::unique_ptr<ASTStmt>(
        static_cast<ASTStmt*>(stmt->instantiate())));

  return new ASTFunction(
    std::unique_ptr<ASTDeclNode>(
      static_cast<ASTDeclNode*>(this->DECL->instantiate())),
    std::move(formals),
    std::move(decls),
    std::move(body));
}
