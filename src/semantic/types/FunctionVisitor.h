#pragma once

#include <memory>
#include <vector>

#include "ASTFunction.h"
#include "ASTVisitor.h"
#include "SymbolTable.h"

class FunctionVisitor : public ASTVisitor {
public:
  explicit FunctionVisitor(SymbolTable* const syms);

  bool visit(ASTFunction* element) override;
  void endVisit(ASTFunction* element) override;

private:
  SymbolTable* const _symbol_table;
  std::vector<ASTFunction*> _polymorphic_fns;
};
