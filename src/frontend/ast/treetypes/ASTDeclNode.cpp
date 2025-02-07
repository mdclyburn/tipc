#include "ASTDeclNode.h"
#include "ASTVisitor.h"

void ASTDeclNode::accept(ASTVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

std::ostream& ASTDeclNode::print(std::ostream &out) const {
  out << getName();
  return out;
}

ASTNode* ASTDeclNode::instantiate() const {
  return new ASTDeclNode(this->NAME);
}
