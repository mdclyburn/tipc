#pragma once

#include <memory>
#include <vector>

#include "ASTFunction.h"
#include "ASTVisitor.h"
#include "SymbolTable.h"

class FunctionVisitor : public ASTVisitor {
public:
  /** Constructor.
   *
   * \param syms Symbol table for the program under analysis.
   */
  explicit FunctionVisitor(SymbolTable* const syms);

  bool visit(ASTFunction* element) override;

private:
  SymbolTable* const _symbol_table;
  // Names of functions identified as polymorphic.
  std::vector<std::string> _polymorphic_fns;
};
