#pragma once

#include "ASTVisitor.h"

class FunctionVisitor : public ASTVisitor {
public:
  FunctionVisitor();

  bool visit(ASTFunction* element) override;
  void endVisit(ASTFunction* element) override;
};
